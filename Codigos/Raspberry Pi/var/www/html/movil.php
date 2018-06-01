<?php 
$pass = $_GET['pass']; 
$dato = $_GET['type']; //Recogemos las variables por URL
$variable = $_GET['trigger'];
if($pass == "KHJH3EWNuxFIxy6ifbgF"){ //Comprobamos la contraseña
	switch ($variable){ //Seleccionamos entre los tres modos: Actuador, FTP o Estado de alimentacion
	case 0: //0 o 1 se escribe en el .txt correspondiente, un 2 devuelve el estado del archivo
		if($dato < 2) {
			$file = fopen("/home/pi/Desktop/monitorizacion/actuador.txt", "r+");
			fwrite($file,$dato);
			fclose($file);
		} else {
			$file = fopen("/home/pi/Desktop/monitorizacion/actuador.txt", "r+");
			echo fgets($file);
			fclose($file);
		}
		break;
	case 1:
		if($dato < 2) {
			$file = fopen("/home/pi/Desktop/monitorizacion/ftp.txt", "r+");
			fwrite($file,$dato);
			fclose($file);
		} else {
			$file = fopen("/home/pi/Desktop/monitorizacion/ftp.txt", "r+");
			echo fgets($file);
			fclose($file);
		}
		break;
	case 2:
		$file = fopen("/home/pi/Desktop/monitorizacion/estado.txt", "r+");
		echo fgets($file);
		fclose($file);
		break;
	}
} else {
	echo 3; //El 3 representa una contraseña mal introducida
}
?>
