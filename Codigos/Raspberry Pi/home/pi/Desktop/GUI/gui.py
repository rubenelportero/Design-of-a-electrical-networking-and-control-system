# Ruben Garcia Segovia - 15/05/2018
import Tkinter as tk  # for Python3 use import tkinter as tk
from PIL import Image
from urllib import urlopen
from StringIO import StringIO
import time

def tiempoactual(): #Imprimimos la hora en la GUI
	reloj.configure (text = time.strftime("%Y-%m-%d %H:%M:%S") )
	reloj.after(1000,tiempoactual) #Y lo hacemos cada segundo
	
def actualizar_imagenes(): #Actualizamos todas las imagenes
	global boton_pulsado   #Boton que indica que grafica estamos representando
	conseguir_imagenes()
	conseguir_imagenes2()
	conseguir_imagenes3()
	conseguir_imagenes4()
	if(boton_pulsado == 1):
		toggle_text4()
	if(boton_pulsado == 2):
		toggle_text5()
	if(boton_pulsado == 3):
		toggle_text6()
	if(boton_pulsado == 4):
		toggle_text7()
	root.after(62000,actualizar_imagenes) #Y lo hacemos cada 62 seg

	

def conseguir_imagenes(): #Grafica de potencia
	URL = 'http://192.168.0.197/grafica_GUI.php?type0=4' #Nos conectamos al servidor
	data = urlopen(URL).read() #Leemos la respuesta del servidor
	file_pot = StringIO(data) #Guardamos como archivo de entrada la respuesta
	img = Image.open(file_pot) #Lo abrimos como imagen
	img.save('./potencias.png') #Guardamos la imagen
	file_pot = None #vaciamos RAM.
	data = None 
	
def conseguir_imagenes2():
	URL2 = 'http://192.168.0.197/grafica_GUI.php?type0=1'
	data2 = urlopen(URL2).read()
	file_frec = StringIO(data2) 
	img2 = Image.open(file_frec)
	img2.save('./frecuencias.png')
	file_frec = None
	data2 = None
	
def conseguir_imagenes3():
	URL3 = 'http://192.168.0.197/grafica_GUI.php?type0=3'
	data3 = urlopen(URL3).read()
	file_fdp = StringIO(data3) 
	img3 = Image.open(file_fdp)
	img3.save('./fdp.png')
	file_fdp = None
	data3 = None
	
def conseguir_imagenes4():
	URL4 = 'http://192.168.0.197/grafica_GUI.php?type0=0'
	data4 = urlopen(URL4).read()
	file_medias = StringIO(data4) 
	img4 = Image.open(file_medias)
	img4.save('./medias.png')
	file_medias = None
	data4 = None

def actuador(funcion): #Cambiamos el estado del actuador
	f = open ('../monitorizacion/actuador.txt','w')
	f.write(funcion)
	f.close()
	
def actuador_lectura(): #Leemos el estado del actuador
	f = open ('../monitorizacion/actuador.txt','r')
	return f.read()
	f.close()

def actuador_boton(): #Comprueba el estado del actuador a cada segundo
	if actuador_lectura() == "1":
		button.configure(text= "Apagar actuador")
	else:
		button.configure(text= "Encender actuador")
	button.after(1000,actuador_boton)
	
def ftp(funcion): #Cambiamos el estado del ftp
	f = open ('../monitorizacion/ftp.txt','w')
	f.write(funcion)
	f.close()
	
def ftp_boton(): #Comprueba el estado del FTP a cada segundo
	if ftp_lectura() == "1":
		button2.configure(text= "Apagar FTP")
	else:
		button2.configure(text= "Encender FTP")
	button2.after(1000,ftp_boton)
	
def ftp_lectura(): #Leemos el estado del FTP
	f = open ('../monitorizacion/ftp.txt','r')
	return f.read()
	f.close()
	
def log_lectura(): #Cargamos en la caja de texto el log entero.
	editArea.delete("1.0",tk.END) #Borramos el texto de la caja de texto
	f = open('../basededatos/log.txt', 'r') #leemos el archivo .txt
	editArea.insert(tk.INSERT,f.read()) #Insertamos el texto (Incluidos saltos de linea)
	editArea.see(tk.END) #Hacemos auto-scroll
	f.close() #Cerramos el archivo.
	root.after(1000,log_lectura) #Ejecutamos esto cada segundo.
	
def toggle_text1(): #Detecta la pulsacion del boton_actuador
    if button["text"] == "Encender actuador":
		actuador("1")
		button["text"] = "Apagar actuador"
    else:
		actuador("0")
		button["text"] = "Encender actuador"
		
def toggle_text2(): #Detecta la pulsacion del boton_FTP
    if button2["text"] == "Encender FTP":
		ftp("1")
		button2["text"] = "Apagar FTP"
    else:
		ftp("0")
		button2["text"] = "Encender FTP"

def toggle_text3(): #Detecta la pulsacion del boton_LOG
	global boton_pulsado
	label.config(image='')
	log_lectura()
	boton_pulsado = 0

def toggle_text4(): #Detecta la pulsacion del boton_potencias
	global boton_pulsado
	image.configure(file="./potencias.png")
	label.config(image=image)
	boton_pulsado = 1
	
def toggle_text5(): #Detecta la pulsacion del boton_Vmedias
	global boton_pulsado
	image.configure(file="./medias.png")
	label.config(image=image)
	boton_pulsado = 2

def toggle_text6(): #Detecta la pulsacion del boton_frecuencias
	global boton_pulsado
	image.configure(file="./frecuencias.png")
	label.config(image=image)
	boton_pulsado = 3

def toggle_text7(): #Detecta la pulsacion del boton_FDP
	global boton_pulsado
	image.configure(file="./fdp.png")
	label.config(image=image)
	boton_pulsado = 4

root = tk.Tk() #Iniciamos el entorno
root.geometry('800x480') #Las mismas dimensiones que la pantalla tactil
root.title("Dispositivo de monitorizacion de red electrica IIOT") #Titulo de la ventana

reloj = tk.Label(root, text="Cargando...")	#Label de la hora
reloj.place(x=330,y=18) #Con sus coordenadas

#Todos los Widget.place hacen alusion a las cordenadas XY absolutas de la ventana 800x480

button = tk.Button( text="Encender actuador", width=30, command=toggle_text1) #Creamos el boton del actuador
button.place(x=30, y = 10)

button2 = tk.Button( text="Apagar FTP", width=30, command=toggle_text2) #Creamos el boton del FTP
button2.place(x= 500, y=10)

actuador_boton() #Comprobamos el estado del actuador
ftp_boton() #Comprobamos el estado del ftp

button3 = tk.Button( text="LOG", width=10, command=toggle_text3) #Creamos el boton del LOG
button3.place(x= 30, y=50)

button4 = tk.Button( text="Potencias", width=10, command=toggle_text4) #Creamos el boton de potencias
button4.place(x= 170, y=50)

button5 = tk.Button( text="Vrms/Irms", width=10, command=toggle_text5) #Creamos el boton Vrms/Irms
button5.place(x= 310, y=50)

button6 = tk.Button( text="Frecuencias", width=10, command=toggle_text6) #Creamos el boton frecuencias
button6.place(x= 460, y=50)

button7 = tk.Button( text="FDP", width=10, command=toggle_text7) #Creamos el boton FTP
button7.place(x= 610, y=50)

#Creacion de la caja de texto para el log
frame1 = tk.Frame(root,width=400, height=480,bg = '#ffffff',borderwidth=1, relief="sunken") #Frame donde se aloja el texto
scrollbar = tk.Scrollbar(frame1) #Creamos el scrollbar
#El texto ira sobre el frame:
editArea = tk.Text(frame1, width=100, height=10, wrap="word",yscrollcommand=scrollbar.set,borderwidth=0, highlightthickness=0)
scrollbar.config(command=editArea.yview) #Scrollbar solo en vertical
scrollbar.pack(side="right", fill="y")
editArea.pack(side="left", fill="both", expand=True)
frame1.place(x=30,y=180)

image = tk.PhotoImage(file="./potencias.png") #Cargamos el widget de imagen
label = tk.Label(image=image)
label.pack()
label.place(x=30,y=80)

toggle_text4() #Autoactivamos el primer boton
tiempoactual() #Encendemos el reloj
actualizar_imagenes() #Actualizamos las imagenes

root.mainloop() 
