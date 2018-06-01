<?php 
$pass = $_GET['pass']; 
$grafica_mostrar = $_GET['type']; 
?>

<DIV align="center">
<?php
if(!($_POST['contra']=="rubenelportero" || isset($_POST['grafica']) || isset($_POST['actuador']) || isset($_POST['ftp']) || $pass == "KHJH3EWNuxFIxy6ifbgF")){
?>	
<form method="POST">
Contraseña <input type="password" name="contra"> <br>
<input type="submit" name="submit" value="Submit">
</form>
<?php
}
if($_POST['contra']=="rubenelportero" || isset($_POST['grafica']) || isset($_POST['actuador']) || $_POST['ftp'] || $pass == "KHJH3EWNuxFIxy6ifbgF"){

if($pass == "KHJH3EWNuxFIxy6ifbgF"){
	$_POST['grafica'] = $grafica_mostrar;
}
header("refresh:20;url=http://192.168.0.197/index.php?pass=KHJH3EWNuxFIxy6ifbgF&type=".$_POST['grafica']."");

?>
<form action="/index.php" method="POST">
<select name="grafica">
	<option value="0" <?php if($_POST['grafica']==0):?> selected="selected" <?php endif?> >Potencias</option>
	<option value="1" <?php if($_POST['grafica']==1):?> selected="selected" <?php endif?>>Voltaje e Intensidad rms</option>
	<option value="3" <?php if($_POST['grafica']==3):?> selected="selected" <?php endif?>>Frecuencias</option>
	<option value="4" <?php if($_POST['grafica']==4):?> selected="selected" <?php endif?>>Factor de potencia</option>
</select>
<input type="submit">
</form>

<?php 
switch($_POST['grafica']){
	case 0:
		echo "<img src='./grafica.php?type0=4'>"; 
		break;
	case 1:
		echo "<img src='./grafica.php?type0=0'>"; 
		echo "<br>";
		break;
	case 3:
		echo "<img src='./grafica.php?type0=1'>"; 
		break;
	case 4:
		echo "<img src='./grafica.php?type0=3'>"; 
		break;
}
?>

<div id="log" style="border : solid 2px #000000;
			backdround : #ffffff;
			color : #000000;
			width : 500px;
			height: 200px;
			overflow : auto; ">
			
<?php   
$myfile = fopen("/home/pi/Desktop/basededatos/log.txt", "r");
while(!feof($myfile))
  {
   echo fgets($myfile);
   echo "<br>";
  }
fclose($myfile);
$file = fopen("/home/pi/Desktop/monitorizacion/actuador.txt", "r");
$actu_txt = fgets($file);
fclose($file);
$file = fopen("/home/pi/Desktop/monitorizacion/ftp.txt", "r");
$ftp_txt = fgets($file);
fclose($file);
if ($actu_txt=="1"){
	$actu_txt = "Activado";
} else {
	$actu_txt = "Desactivado";
}
if ($ftp_txt=="1"){
	$ftp_txt = "Activado";
} else {
	$ftp_txt = "Desactivado";
}
?>

</div>


<?php


$espaciolibre = disk_free_space("/");
$espaciototal = disk_total_space("/");
$espacioporcentaje = ($espaciolibre/$espaciototal)*100;
echo "Espacio disponible para almacenamiento: ";
echo intval($espacioporcentaje);
echo "%";

$file2 = fopen("/home/pi/Desktop/monitorizacion/actuador.txt", "r+");
if($_POST['actuador']=="Activar"){
	$actu_txt = "Activado";
	fwrite($file2,"1");
} else if($_POST['actuador']=="Desactivar"){
	$actu_txt = "Desactivado";
	fwrite($file2,"0");
}
fclose($file2);

$file3 = fopen("/home/pi/Desktop/monitorizacion/ftp.txt", "r+");
if($_POST['ftp']=="Activar"){
	$ftp_txt = "Activado";
	fwrite($file3,"1");
} else if($_POST['ftp']=="Desactivar"){
	$ftp_txt = "Desactivado";
	fwrite($file3,"0");
}
fclose($file3);

?>
<form  action="/index.php" method="POST">
<br>Estado del actuador: <?php echo $actu_txt?> <br>
<input type="hidden" name="actuador" value="hola!">
<input type="radio" name="actuador" value="Activar">Activar
<input type="radio" name="actuador" value="Desactivar">Desactivar<br>
<input type="submit" name="submit" value="Enviar">
</form>

<form  action="/index.php" method="POST">
Estado del ftp: <?php echo $ftp_txt?> <br>
<input type="hidden" name="ftp" value="hola!">
<input type="radio" name="ftp" value="Activar">Activar
<input type="radio" name="ftp" value="Desactivar">Desactivar<br>
<input type="submit" name="submit" value="Enviar">
</form>
<?php echo date(DATE_RFC2822);
}
?>
</DIV>
