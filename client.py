import Tkinter as tk
import os
import Image, ImageTk

#globals
model = " none"
command = "./pclient localhost 10020 "

def invokeClient(model):
	os.system( command + model)
	
	

#callbacks 
def click1():
	global model
	button1.config(bg="green")
	button1.config(activebackground="green")
	button2.config(bg="white")
	button2.config(activebackground="white")
	button3.config(bg="white")
	button3.config(activebackground="white")
	model=" dragon"
	invokeClient(model)
	

def click2():
	global model
	button2.config(bg="green")
	button2.config(activebackground="green")
	button1.config(bg="white")
	button1.config(activebackground="white")
	button3.config(bg="white")
	button3.config(activebackground="white")
	model=" happy"
	invokeClient(model)
	
def click3():
	global model
	button3.config(bg="green")
	button3.config(activebackground="green")
	button2.config(bg="white")
	button2.config(activebackground="white")
	button1.config(bg="white")
	button1.config(activebackground="white")
	model=" huge"
	invokeClient(model)
	
#main window config
root = tk.Tk()
root.title("Reciever driven 3D Streaming")
#root.minsize(400, 400)
root.geometry('600x400+400+200')

frame1 = tk.Frame(root)
frame1.pack(expand=1,side=tk.TOP, fill=tk.X)

#button images
path = os.getcwd()
photo1 = tk.PhotoImage(file=path + "/dragon.gif")
photo2 = tk.PhotoImage(file=path + "/happy.gif")

im = Image.open(path + "/thai.jpg")
#ImageTk.PhotoImage("RGBA",(80,80))
photo3 = ImageTk.PhotoImage(im)


#main label
label1 = tk.Label(compound=tk.TOP,takefocus=0,text="Select a Model to Start Streaming",font = ("Arial", 12, "bold"))
label1.pack(side=tk.TOP, padx=2, pady=2)


#buttons
button1 = tk.Button(frame1, compound=tk.TOP, image=photo1,text="Dragon" , bg='white',activebackground='white' , command=click1)
button1.pack(side=tk.LEFT, padx=2, pady=2)

button2 = tk.Button(frame1, compound=tk.TOP, image=photo2,text="Happy Buddha" , bg='white', activebackground='white', command=click2)
button2.pack(side=tk.LEFT, padx=2, pady=2)

button3 = tk.Button(frame1, compound=tk.TOP, image=photo3,text=" Thai Statue" , bg='white', activebackground='white', command=click3)
button3.pack(side=tk.LEFT, padx=2, pady=2)



button1.image = photo1
button2.image = photo2
button3.image = photo3 

# start the event loop
root.mainloop()
