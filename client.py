import Tkinter as tk
import os
import Image, ImageTk
import datetime

#globals
model = " huge "
command = "./pclient localhost 10020 "


def invokeClient(model):
	#name=text1.get()
	#if name=="Enter Name":
	#	name=" DEFAULT "
	#print command + model + " view_config " + name 
	os.system( command + model + " view_config " )
	

def child1View1():
	global model
	model=" dragon1"
	text_file.write(str(datetime.datetime.now()))
	text_file.write(" View Item Dragon : Item # 1432 \n")
	invokeClient(model)

def child1View2():
	global model
	model=" dragon2"
	text_file.write(str(datetime.datetime.now()))
	text_file.write(" View Item Dragon : Item # 1433 \n")
	invokeClient(model)

def child1View3():
	global model
	model=" dragon3"
	text_file.write(str(datetime.datetime.now()))
	text_file.write(" View Item Dragon : Item # 1434 \n")
	invokeClient(model)
	
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
	
def makeentry(parent, caption, width=None, **options):
    	tk.Label(parent, text=caption).pack(side=tk.LEFT)
    	entry = tk.Entry(parent, **options)
    	if width:
        	entry.config(width=width)
    	entry.pack(side=tk.LEFT)
    	return entry
 

def buyDragon1():
	if(dragonbuy1.get()=="Buy") :
		text_file.write(str(datetime.datetime.now()))
		text_file.write(" Buy Item Dragon : Item # 1432 \n")
		buttonap1.configure(bg='green')
		dragonbuy1.set('Cancel')
	else :
		text_file.write(str(datetime.datetime.now()))
		text_file.write(" Cancel Item Dragon : Item # 1432 \n")
		buttonap1.configure(bg='red')
		dragonbuy1.set('Buy')
	
def buyDragon2():
	if(dragonbuy2.get()=="Buy") :
		text_file.write(str(datetime.datetime.now()))
		text_file.write(" Buy Item Dragon : Item # 1433 \n")
		buttonap2.configure(bg='green')
		dragonbuy2.set('Cancel')
	else :
		text_file.write(str(datetime.datetime.now()))
		text_file.write(" Cancel Item Dragon : Item # 1433 \n")
		buttonap2.configure(bg='red')
		dragonbuy2.set('Buy')
	
def buyDragon3():
	if(dragonbuy3.get()=="Buy") :
		text_file.write(str(datetime.datetime.now()))
		text_file.write(" Buy Item Dragon : Item # 1434 \n")
		buttonap3.configure(bg='green')
		dragonbuy3.set('Cancel')
	else :
		text_file.write(str(datetime.datetime.now()))
		text_file.write(" Cancel Item Dragon : Item # 1434 \n")
		buttonap3.configure(bg='red')
		dragonbuy3.set('Buy')
	
def mainWindow2Child1():
	#global board
	root.withdraw()
	child1.deiconify()
	text_file.write(str(datetime.datetime.now()))
	text_file.write(" Select Catagory : Dragon \n")

def child1ToMainWindow():
	#global board
	child1.withdraw()
	root.deiconify()
	text_file.write(str(datetime.datetime.now()))
	text_file.write(" Exit to Main Window from Catagory : Dragon \n")

#def childWindow1():
	#board = tk.Toplevel()
	#board.geometry('600x400+400+200')
	#board.title("Window 2")
	#startButton1 = tk.Button(board, text="Start", command=test)
	#startButton1.place(x=290, y= 100, width=300, height=20)
	


#main

root = tk.Tk()
root.title("The Premier Online Antiques Shop")
root.geometry('800x600+250+0')

root.configure(bg="#01008F")

child1 = tk.Toplevel()
child1.geometry('800x600+250+0')
child1.title("The Premier Online Antiques Shop")
backbut1 = tk.Button(child1, text="<--BACK", command=child1ToMainWindow,bg='gray',activebackground='yellow')
backbut1.place(x=20, y= 540)
child1.protocol("WM_DELETE_WINDOW", child1ToMainWindow)
child1.configure(bg="#01008F")
child1.withdraw()


canvas1 = tk.Canvas(width = 200, height = 300, bg = 'white')
canvas1.place(x=50,y=60)
canvas2 = tk.Canvas(width = 200, height = 300, bg = 'white')
canvas2.place(x=300,y=60)
canvas3 = tk.Canvas(width = 200, height = 300, bg = 'white')
canvas3.place(x=540,y=60)

e111 = tk.Label(child1,relief='ridge',text="Seller :   www.Macys.com",anchor='w',bg='white')
#e.grid(row=i, column=j, sticky='nsew')
#e11.insert('end', 'Seller: Macys.com')
e111.place(x=50,y= 280,width=200)

e112 = tk.Label(child1,relief='ridge',text="Stock :   Available",anchor='w',bg='white')
#e.grid(row=i, column=j, sticky='nsew')
#e12.insert('end', 'Stock : Available')
e112.place(x=50,y= 305,width=200)

e113 = tk.Label(child1,relief='ridge',text="Price :  SG$ 23.00",anchor='w',bg='white')
e113.place(x=50,y= 330,width=200)

e121 = tk.Label(child1,relief='ridge',text="Seller :   Barnes & Noble",anchor='w',bg='white')
#e.grid(row=i, column=j, sticky='nsew')
#e11.insert('end', 'Seller: Macys.com')
e121.place(x=280,y= 280,width=200)


e122 = tk.Label(child1,relief='ridge',text="Stock :   Available",anchor='w',bg='white')
#e.grid(row=i, column=j, sticky='nsew')
#e12.insert('end', 'Stock : Available')
e122.place(x=280,y= 305,width=200)

e123 = tk.Label(child1,relief='ridge',text="Price :  SG$ 23.00",anchor='w',bg='white')
e123.place(x=280,y= 330,width=200)

e131 = tk.Label(child1,relief='ridge',text="Seller :   Private Collector",anchor='w',bg='white')
#e.grid(row=i, column=j, sticky='nsew')
#e11.insert('end', 'Seller: Macys.com')
e131.place(x=500,y= 280,width=200)

e132 = tk.Label(child1,relief='ridge',text="Stock :   Available",anchor='w',bg='white')
#e.grid(row=i, column=j, sticky='nsew')
#e12.insert('end', 'Stock : Available')
e132.place(x=500,y= 305,width=200)

e133 = tk.Label(child1,relief='ridge',text="Price :  SG$ 23.00",anchor='w',bg='white')
e133.place(x=500,y= 330,width=200)


buttona1 = tk.Button(child1, compound=tk.TOP,bg='gray' ,activebackground='yellow', text="View Item #1432", command=child1View1)
buttona1.place(x=90, y= 360)
buttona2 = tk.Button(child1, compound=tk.TOP,bg='gray' , activebackground='yellow', text="View Item #1433", command=child1View2)
buttona2.place(x=320, y= 360)
buttona3 = tk.Button(child1, compound=tk.TOP,bg='gray' ,activebackground='yellow', text="View Item #1434", command=child1View3)
buttona3.place(x=540, y= 360)

dragonbuy1 = tk.StringVar()
dragonbuy2 = tk.StringVar()
dragonbuy3 = tk.StringVar()
dragonbuy1.set('Buy')
dragonbuy2.set('Buy')
dragonbuy3.set('Buy')

buttonap1 = tk.Button(child1, compound=tk.TOP,bg='red' ,activebackground='yellow', textvariable=dragonbuy1, command=buyDragon1)
buttonap1.place(x=130, y= 400)
buttonap2 = tk.Button(child1, compound=tk.TOP,bg='red' , activebackground='yellow', textvariable=dragonbuy2, command=buyDragon2)
buttonap2.place(x=360, y= 400)
buttonap3 = tk.Button(child1, compound=tk.TOP,bg='red' ,activebackground='yellow', textvariable=dragonbuy3, command=buyDragon3)
buttonap3.place(x=580, y= 400)
#frame1 = tk.Frame(root)
#frame1.pack(expand=1,side=tk.TOP, fill=tk.X)

#button images
path = os.getcwd()
photo1 = tk.PhotoImage(file=path + "/dragon.gif")
photo2 = tk.PhotoImage(file=path + "/happy.gif")

im = Image.open(path + "/thai.jpg")
photo3 = ImageTk.PhotoImage(im)

label11 = tk.Label(child1,takefocus=0,text="Available Statues of : Dragon",font = ("Arial", 18, "bold"),bg="#01008F")
label11.pack(side=tk.TOP, padx=2, pady=2)
label11.place(x=160, y= 20, width=450, height=30)

pic11 = tk.Label(child1, compound=tk.TOP, image=photo1,text="Dragon: Item #1432" , bg='white',activebackground='white')
pic11.place(x=50, y= 60)
pic12 = tk.Label(child1, compound=tk.TOP, image=photo1,text="Dragon: Item #1433" , bg='yellow',activebackground='white')
pic12.place(x=280, y= 60)
pic13 = tk.Label(child1, compound=tk.TOP, image=photo1,text="Dragon: Item #1434" , bg='purple',activebackground='white')
pic13.place(x=500, y= 60)

#main label
label1 = tk.Label(root,takefocus=0,text="Welcome to the online Antiques Shop",font = ("Arial", 18, "bold"),bg="#01008F")

label1.pack(side=tk.TOP, padx=2, pady=2)
label1.place(x=120, y= 20, width=570, height=30)

#text1 =  makeentry(root, "Session name:", 15)
#text1.insert(0, "Enter Name")
##text1.pack(side=tk.TOP, padx=2, pady=2)
##text1.pack()

#buttons
#pic1 = tk.Label(root, compound=tk.TOP, image=photo1,text="Dragon" , bg='purple',activebackground='white')
#button1.pack(side=tk.LEFT, padx=2, pady=2)
#pic1.place(x=50, y= 60)
canvas1.create_image(6, 70, image = photo1, anchor = 'nw')
canvas2.create_image(50, 30, image = photo2, anchor = 'nw')
canvas3.create_image(25, 1, image = photo3, anchor = 'nw')

dis1 =  tk.Label(root,takefocus=0,text="Dragon : is a legendary creature with serpentine or otherwise reptilian traits that features in the myths of many cultures. The statue is of Chinese Dragon which is said to give good luck to the owner",wraplength=180,bg="white",relief='raised')
dis1.place(x=50, y= 400,width=200, height=120,)

button1 = tk.Button(root, compound=tk.TOP, bg='gray',activebackground='yellow',text="View Item",command=mainWindow2Child1)
button1.place(x=100, y= 540)


#pic2 = tk.Label(root, compound=tk.TOP, image=photo2,text="Happy Buddha" , bg='purple', activebackground='white')
#button2.pack(side=tk.LEFT, padx=2, pady=2)
#pic2.place(x=345, y= 60)
dis2 =  tk.Label(root,takefocus=0,text="Laughing Buddha : is a symbol of happiness, contentment, and prosperity. He is bald a rotund figure with a warm smiling face. He is clothed in robes, and his protruding tummy symbolizes abundance and mirth. ",wraplength=180,bg="white",relief='raised')
dis2.place(x=300, y= 400,width=200, height=120)

button2 = tk.Button(root, compound=tk.TOP, bg='gray',activebackground='yellow',text="View Item")
button2.place(x=350, y= 540)


#pic3 = tk.Label(root, compound=tk.TOP, image=photo3,text=" Thai Statue" , bg='purple', activebackground='white')
#button3.pack(side=tk.LEFT, padx=2, pady=2)
#pic3.place(x=560, y= 60)
dis3 =  tk.Label(root,takefocus=0,text="Thai Statue : is a symbol of devotion .The Dragon heads at the foot of the statue represent the evil of the world and the eliphants are the mythical creatures that are absolvers of humans who stand atop.",wraplength=180,bg="white",relief='raised')
dis3.place(x=540, y= 400,width=200, height=120)

button3 = tk.Button(root, compound=tk.TOP, bg='gray',activebackground='yellow',text="View Item")
button3.place(x=600, y= 540)


#pic1.image = photo1
#pic2.image = photo2
#pic3.image = photo3 

# start the event loop
text_file = open("log-"+str(datetime.datetime.now())+".txt", "w")

root.mainloop()


