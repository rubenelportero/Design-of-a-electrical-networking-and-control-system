<?php  
//Ejemplo de uso en la libreria Pchart Modificado por 
//Ruben Garcia Segovia
//Version GUI (resolucion distinta)
 /* Set the default timezone */
 date_default_timezone_set('Etc/GMT');

 /* pChart library inclusions */
 include("./library/pchart2/class/pData.class.php");
 include("./library/pchart2/class/pDraw.class.php");
 include("./library/pchart2/class/pImage.class.php");
 
  /* Create and populate the pData object */
 $MyData = new pData();
 $BaseTs = mktime(0,0,0,4,18,2018);
 $fecha = getdate();
 
$tipo = $_GET['type0'];  //Cogemos la grafica a representar por la url

//Con esta matriz representamos las distintas graficas dependiendo del valor obtenido por url.
$type[0][0] = "VRMS1";
$type[0][1] = "IRMS1";
$type[0][2] = "Valores rms";
$type[1][0] = "FrecV1";
$type[1][1] = "FrecI1";
$type[1][2] = "Frecuencias de onda";
$type[2][0] = "PicoV1";
$type[2][1] = "PicoI1";
$type[2][2] = "Valores maximos";
$type[3][0] = "FdpL1";
$type[3][1] = "";
$type[3][2] = "Factor de potencia L1";
$type[4][0] = "PotActL1";
$type[4][1] = "PotReactL1";
$type[4][2] = "Potencias L1";

//Leemos el archivo con los datos a representar
$myfile = fopen("/home/pi/Desktop/basededatos/".$fecha[year]."/".$fecha[mon]."/".$fecha[mday]."/".$type[$tipo][0].".txt", "r");
$datos = array();
$tiempos = array();
$i = 0;
 
while(!feof($myfile)) //Hasta llegar al final del archivo
  {
   $leer = fgets($myfile);
   list($tiempo, $dato) =  explode(" ",$leer); //Separamos la linea por el espacio
   $datos[$i] = $dato; //Obtenemos el dato
   $tiempos[$i] = $tiempo; //Obtenemos el tiempo
   $i = $i + 1;
  }
  
fclose($myfile); 

if($tipo != 3) { //Depende de la representacion a hacer, tenemos que recoger mas de una grafica
$b = 0;
$myfile2 = fopen("/home/pi/Desktop/basededatos/".$fecha[year]."/".$fecha[mon]."/".$fecha[mday]."/".$type[$tipo][1].".txt", "r");
$datos2 = array();
while(!feof($myfile2))
  {
   $leer = fgets($myfile2);
   list($tiempo, $dato) =  explode(" ",$leer);
   $datos2[$b] = $dato;
   $b = $b + 1;
  }
  
fclose($myfile2); 
}

if($tipo == 4) { //Depende de la representacion a hacer, tenemos que recoger mas de una grafica
$c = 0;
$myfile3 = fopen("/home/pi/Desktop/basededatos/".$fecha[year]."/".$fecha[mon]."/".$fecha[mday]."/PotaparL1.txt", "r");
$datos3 = array();
while(!feof($myfile3))
  {
   $leer = fgets($myfile3);
   list($tiempo, $dato) =  explode(" ",$leer);
   $datos3[$c] = $dato;
   $c = $c + 1;
  }
  
fclose($myfile3); 
}

$i = $i - 1;
$z = 10;
$y=0;
if($i < 30){ //Dependiendo del numero de muestras, representamos mas o menos valores de tiempo
	$f=1;  //La representacion en la misma, pero con menos muestras se ve el tiempo en mas detalle
} else if ($i < 120){
	$f = 4;
} else if($i < 240){
	$f = 9;
} else {
	$f = 59;	
}
for($j=0; $j<$i; $j++)
{
	$MyData->addPoints($datos[$j],"".$type[$tipo][0].""); //Introducimos un valor a la grafica
	if($tipo!=3){
		$MyData->addPoints($datos2[$j],"".$type[$tipo][1].""); //Introducimos un valor a la grafica
	}
	if($tipo==4){
		$MyData->addPoints($datos3[$j],"PotaparL1"); //Introducimos un valor a la grafica
	}
	if($z>$f){
		$MyData->addPoints($BaseTs + $tiempos[$j]*60,"Tiempo"); //Introducimos un valor al tiempo
		$y = $j;
		$z=1;
	} else {
		$z = $z + 1;
		$MyData->addPoints($BaseTs + $tiempos[$y]*60,"Tiempo"); //Introducimos un valor al tiempo
	}
		
}
  
$MyData->setAxisName(0,"Y");
$MyData->setAxisDisplay(0,AXIS_FORMAT_METRIC); //Formato metrico de representacion
$MyData->setAbscissa("Tiempo");
$MyData->setXAxisDisplay(AXIS_FORMAT_TIME,"H:i"); //El tiempo en HH:MM 

/* Create the pChart object */
$myPicture = new pImage(750,390,$MyData);//La resolucion de la imagen optima para la GUI
 
/* Draw a background */
$Settings = array("R"=>90, "G"=>90, "B"=>90, "Dash"=>1, "DashR"=>120, "DashG"=>120, "DashB"=>120);
$myPicture->drawFilledRectangle(0,0,750,390,$Settings); 

/* Overlay with a gradient */ 
$Settings = array("StartR"=>200, "StartG"=>200, "StartB"=>200, "EndR"=>50, "EndG"=>50, "EndB"=>50, "Alpha"=>50);
$myPicture->drawGradientArea(0,0,750,390,DIRECTION_VERTICAL,$Settings); 
$myPicture->drawGradientArea(0,0,750,390,DIRECTION_HORIZONTAL,$Settings); 

/* Add a border to the picture */
$myPicture->drawRectangle(0,0,749,389,array("R"=>0,"G"=>0,"B"=>0));
 
/* Write the chart title */ 
$myPicture->setFontProperties(array("FontName"=>"./library/pchart2/fonts/Forgotte.ttf","FontSize"=>11));
$myPicture->drawText(150,35,"".$type[$tipo][2]."",array("FontSize"=>20,"Align"=>TEXT_ALIGN_BOTTOMMIDDLE));

/* Set the default font */
$myPicture->setFontProperties(array("FontName"=>"./library/pchart2/fonts/pf_arma_five.ttf","FontSize"=>6));

/* Define the chart area */
$myPicture->setGraphArea(30,40,740,375);

/* Draw the scale */
$scaleSettings = array("XMargin"=>10,"YMargin"=>10,"Floating"=>TRUE,"GridR"=>200,"GridG"=>200,"GridB"=>200,"RemoveSkippedAxis"=>TRUE,"DrawSubTicks"=>FALSE,"Mode"=>SCALE_MODE_ADDALL_START0,"LabelingMethod"=>LABELING_DIFFERENT);

$myPicture->drawScale($scaleSettings);

/* Draw the line chart */
$myPicture->setShadow(TRUE,array("X"=>1,"Y"=>1,"R"=>0,"G"=>0,"B"=>0,"Alpha"=>10));
$myPicture->drawLineChart();

/* Write the chart legend */
$myPicture->drawLegend(580,20,array("Style"=>LEGEND_NOBORDER,"Mode"=>LEGEND_HORIZONTAL));

/* Render the picture (choose the best way) */
$myPicture->autoOutput("pictures/example.drawSplineChart.network.png");

?>
