import Tkinter as tk
import os
import Image, ImageTk
import datetime
import random

model = "huge "
command = "./pclient localhost 10020 "


def invokeClient(model):
    #name=text1.get()
    #if name=="Enter Name":
    #    name=" DEFAULT "
    
    name = "experiment "
    global command
    global delay
    global data_rate
    #print command + model + " view_config " + name + delay + data_rate 
    os.system( command + model + " view_config " + name + delay + data_rate)

   
def child2View1():
    global model
    model=" happy_damage1"
    text_file.write(str(datetime.datetime.now()))
    text_file.write(" View Item happy_damage1 : Item # 1532 \n")
    invokeClient(model)

def child2View2():
    global model
    model=" happy_damage2"
    text_file.write(str(datetime.datetime.now()))
    text_file.write(" View Item happy_damage2 : Item # 1533 \n")
    invokeClient(model)

def child2View3():
    global model
    model=" happy"
    text_file.write(str(datetime.datetime.now()))
    text_file.write(" View Item happy : Item # 1534 \n")
    invokeClient(model)
   
def child3View1():
    global model
    model=" huge_damage1"
    text_file.write(str(datetime.datetime.now()))
    text_file.write(" View Item thai_damage1 : Item # 1632 \n")
    invokeClient(model)

def child3View2():
    global model
    model=" huge"
    text_file.write(str(datetime.datetime.now()))
    text_file.write(" View Item thai : Item # 1633 \n")
    invokeClient(model)

def child3View3():
    global model
    model=" huge_damage2"
    text_file.write(str(datetime.datetime.now()))
    text_file.write(" View Item thai_damage2 : Item # 1634 \n")
    invokeClient(model)
   
def child1View1():
    global model
    model=" dragon"
    text_file.write(str(datetime.datetime.now()))
    text_file.write(" View Item dragon : Item # 1432 \n")
    invokeClient(model)

def child1View2():
    global model
    model=" dragon_damage1"
    text_file.write(str(datetime.datetime.now()))
    text_file.write(" View Item dragon_damage1 : Item # 1433 \n")
    invokeClient(model)

def child1View3():
    global model
    model=" dragon_damage2"
    text_file.write(str(datetime.datetime.now()))
    text_file.write(" View Item dragon_damage2 : Item # 1434 \n")
    invokeClient(model)
  

def buyThai1():
    if(thaibuy1.get()=="Buy") :
        text_file.write(str(datetime.datetime.now()))
        text_file.write(" Buy Item thai_damage1 : Item # 1632 \n")
        #child6_buy1.configure(bg='green')
        thaibuy1.set('Cancel')
    else :
        text_file.write(str(datetime.datetime.now()))
        text_file.write(" Cancel Item thai_damage1 : Item # 1632 \n")
        #child6_buy1.configure(bg='red')
        thaibuy1.set('Buy')
   
def buyThai2():
    if(thaibuy2.get()=="Buy") :
        text_file.write(str(datetime.datetime.now()))
        text_file.write(" Buy Item thai : Item # 1633 \n")
        #child6_buy2.configure(bg='green')
        thaibuy2.set('Cancel')
    else :
        text_file.write(str(datetime.datetime.now()))
        text_file.write(" Cancel Item thai : Item # 1633 \n")
        #child6_buy2.configure(bg='red')
        thaibuy2.set('Buy')
   
def buyThai3():
    if(thaibuy3.get()=="Buy") :
        text_file.write(str(datetime.datetime.now()))
        text_file.write(" Buy Item thai_damage2 : Item # 1634 \n")
        #child6_buy3.configure(bg='green')
        thaibuy3.set('Cancel')
    else :
        text_file.write(str(datetime.datetime.now()))
        text_file.write(" Cancel Item thai_damage2 : Item # 1634 \n")
        #child6_buy3.configure(bg='red')
        thaibuy3.set('Buy')
       
        
def buyHappy1():
    if(happybuy1.get()=="Buy") :
        text_file.write(str(datetime.datetime.now()))
        text_file.write(" Buy Item happy_damage1 : Item # 1532 \n")
        #child5_buy1.configure(bg='green')
        happybuy1.set('Cancel')
    else :
        text_file.write(str(datetime.datetime.now()))
        text_file.write(" Cancel Item happy_damage1 : Item # 1532 \n")
        #child5_buy1.configure(bg='red')
        happybuy1.set('Buy')
   
def buyHappy2():
    if(happybuy2.get()=="Buy") :
        text_file.write(str(datetime.datetime.now()))
        text_file.write(" Buy Item happy_damage2 : Item # 1533 \n")
        #child5_buy2.configure(bg='green')
        happybuy2.set('Cancel')
    else :
        text_file.write(str(datetime.datetime.now()))
        text_file.write(" Cancel Item happy_damage2 : Item # 1533 \n")
        #child5_buy2.configure(bg='red')
        happybuy2.set('Buy')
   
def buyHappy3():
    if(happybuy3.get()=="Buy") :
        text_file.write(str(datetime.datetime.now()))
        text_file.write(" Buy Item happy : Item # 1534 \n")
        #child5_buy3.configure(bg='green')
        happybuy3.set('Cancel')
    else :
        text_file.write(str(datetime.datetime.now()))
        text_file.write(" Cancel Item happy : Item # 1534 \n")
        #child5_buy3.configure(bg='red')
        happybuy3.set('Buy')
       
       
def buyDragon1():
    if(dragonbuy1.get()=="Buy") :
        text_file.write(str(datetime.datetime.now()))
        text_file.write(" Buy Item dragon : Item # 1432 \n")
        #child4_buy1.configure(bg='green')
        dragonbuy1.set('Cancel')
    else :
        text_file.write(str(datetime.datetime.now()))
        text_file.write(" Cancel Item dragon : Item # 1432 \n")
        #child4_buy1.configure(bg='red')
        dragonbuy1.set('Buy')
   
def buyDragon2():
    if(dragonbuy2.get()=="Buy") :
        text_file.write(str(datetime.datetime.now()))
        text_file.write(" Buy Item dragon_damage1 : Item # 1433 \n")
        #child4_buy2.configure(bg='green')
        dragonbuy2.set('Cancel')
    else :
        text_file.write(str(datetime.datetime.now()))
        text_file.write(" Cancel Item dragon_damage1 : Item # 1433 \n")
        #child4_buy2.configure(bg='red')
        dragonbuy2.set('Buy')
   
def buyDragon3():
    if(dragonbuy3.get()=="Buy") :
        text_file.write(str(datetime.datetime.now()))
        text_file.write(" Buy Item dragon_damage2 : Item # 1434 \n")
        #child4_buy3.configure(bg='green')
        dragonbuy3.set('Cancel')
    else :
        text_file.write(str(datetime.datetime.now()))
        text_file.write(" Cancel Item dragon_damage2 : Item # 1434 \n")
        #child4_buy3.configure(bg='red')
        dragonbuy3.set('Buy')
       
       
def doNothing():
   print "do nothing"


   
def mainWindow2Child():
   global randomseq
   global choice
   if randomseq == []:
       randomseq = [1,2,3]
   choice = random.choice(randomseq)
   randomseq.remove(choice)
   if choice == 1:
       mainWindow2Child1()
   elif choice == 2:
       mainWindow2Child2()
   elif choice == 3:
       mainWindow2Child3()


def mainWindow2Child1():
      root.withdraw()
      child1.deiconify()
      text_file.write(str(datetime.datetime.now()))
      text_file.write(" View Catagory : Dragon \n")

def mainWindow2Child2():
      root.withdraw()
      child2.deiconify()
      text_file.write(str(datetime.datetime.now()))
      text_file.write(" View Catagory : Happy \n")

def mainWindow2Child3():
      root.withdraw()
      child3.deiconify()
      text_file.write(str(datetime.datetime.now()))
      text_file.write(" View Catagory : Thai \n")


def childToMainWindow():
    global choice
    global randomseq
    
    if choice == 4 :
        child4.withdraw()
        text_file.write(str(datetime.datetime.now()))
        text_file.write(" Confirm Purchase of Dragon \n")
     
    elif choice == 5 :
        child5.withdraw()
        text_file.write(str(datetime.datetime.now()))
        text_file.write(" Confirm Purchase of Happy \n")
   
    elif choice == 6 :
        child6.withdraw()
        text_file.write(str(datetime.datetime.now()))
        text_file.write(" Confirm Purchase of Thai \n")

    if randomseq != []:
        mainWindow2Child()
       
    else:
        choice = 0
        root.deiconify()
   
  
def childToChild():
    global choice
    if choice == 1:
        choice = 4
        child1ToChild()
    elif choice == 2:
        choice = 5
        child2ToChild()
    elif choice == 3:
        choice = 6
        child3ToChild()
       


def child1ToChild():
    child1.withdraw()
    child4.deiconify()
    text_file.write(str(datetime.datetime.now()))
    text_file.write(" View items in Dragon \n")
 
def child2ToChild():
    child2.withdraw()
    child5.deiconify()
    text_file.write(str(datetime.datetime.now()))
    text_file.write(" View items in Happy \n")
   
def child3ToChild():
    child3.withdraw()
    child6.deiconify()
    text_file.write(str(datetime.datetime.now()))
    text_file.write(" View items in Thai \n")


#main block starts hear 
root = tk.Tk()
root.title("The Premier Online Antiques Store")
root.geometry('800x600+340+240')
root.resizable(0,0)
root.configure(bg="#CCCC99")
randomseq = [1,2,3]
choice = 0
path = os.getcwd()
photo1 = tk.PhotoImage(file=path + "/dragon.gif")
photo2 = tk.PhotoImage(file=path + "/happy.gif")
im = Image.open(path + "/thai.jpg")
photo3 = ImageTk.PhotoImage(im)


#main window
main_label_1 = tk.Label(root,takefocus=0,text="Welcome to the Online Antiques Store",font = ("Arial", 28, "bold"),bg="#CCCC99")
main_label_1.pack(side=tk.TOP, padx=2, pady=2)
main_label_1.place(x=100, y= 150, width=640, height=40)

main_button_1 = tk.Button(root,compound=tk.TOP,bg='gray',activebackground='yellow',text="Purchase Items",command=mainWindow2Child)
main_button_1.place(x=350, y= 240)


#child1
child1 = tk.Toplevel()
child1.geometry('800x600+340+240')
child1.title("The Premier Online Antiques Shop 1")
child1_backbut1 = tk.Button(child1, text="View Items",command=childToChild,bg='gray',activebackground='yellow')
child1_backbut1.place(x=350, y= 400)
child1.protocol("WM_DELETE_WINDOW", doNothing)
child1.resizable(0,0)
child1.configure(bg="#CCCC99")
child1.withdraw()
child1_pic = tk.Label(child1, compound=tk.TOP, image=photo1,text="Dragon" , bg='white')
child1_pic.place(x=300, y= 60)
child1_display_text =  tk.Label(child1,takefocus=0,text="Dragon : is a legendary creature with serpentine or otherwise reptilian traits that features in the myths of many cultures. The statue is of Chinese Dragon which is said to give good luck to the owner",wraplength=180,bg="white",relief='raised')
child1_display_text.place(x=300, y= 260,width=200, height=120,)


#child2
child2 = tk.Toplevel()
child2.geometry('800x600+340+240')
child2.title("The Premier Online Antiques Shop 2")
child2_backbut1 = tk.Button(child2, text="View Items",command=childToChild,bg='gray',activebackground='yellow')
child2_backbut1.place(x=360, y= 480)
child2.protocol("WM_DELETE_WINDOW", doNothing)
child2.resizable(0,0)
child2.configure(bg="#CCCC99")
child2.withdraw()
child2_pic = tk.Label(child2, compound=tk.TOP, image=photo2,text="Happy Buddha" , bg='white')
child2_pic.place(x=350, y= 60)
child2_display_text =  tk.Label(child2,takefocus=0,text="Laughing Buddha : is a symbol of happiness, contentment, and prosperity. He is bald a rotund figure with a warm smiling face. He is clothed in robes, and his protruding tummy symbolizes abundance and mirth. ",wraplength=180,bg="white",relief='raised')
child2_display_text.place(x=300, y= 340,width=200, height=120)



#child3
child3 = tk.Toplevel()
child3.geometry('800x600+340+240')
child3.title("The Premier Online Antiques Shop 3")
child3_backbut1 = tk.Button(child3, text="View Items",command=childToChild,bg='gray',activebackground='yellow')
child3_backbut1.place(x=370, y= 540)
child3.protocol("WM_DELETE_WINDOW", doNothing)
child3.resizable(0,0)
child3.configure(bg="#CCCC99")
child3.withdraw()
child3_pic = tk.Label(child3, compound=tk.TOP, image=photo3,text=" Thai Statue" , bg='white')
child3_pic.place(x=340, y= 60)
child3_display_text =  tk.Label(child3,takefocus=0,text="Thai Statue : is a symbol of devotion .The Dragon heads at the foot of the statue represent the evil of the world and the eliphants are the mythical creatures that are absolvers of humans who stand atop.",wraplength=180,bg="white",relief='raised')
child3_display_text.place(x=320, y= 400,width=200, height=120)



#child4
child4 = tk.Toplevel()
child4.geometry('800x600+340+240')
child4.title("The Premier Online Antiques Shop 4")
child4_viewitems = tk.Button(child4, text="Confirm Purchase -->",command=childToMainWindow,bg='gray',activebackground='yellow')
child4_viewitems.place(x=580, y= 540)
child4.protocol("WM_DELETE_WINDOW", doNothing)
child4.resizable(0,0)
child4.configure(bg="#CCCC99")
child4.withdraw()
##
child4_pic1 = tk.Label(child4, compound=tk.TOP, image=photo1,text="Dragon: Item #1432" , bg='white')
child4_pic1.place(x=50, y= 60)
child4_pic2 = tk.Label(child4, compound=tk.TOP, image=photo1,text="Dragon: Item #1433" , bg='white')
child4_pic2.place(x=280, y= 60)
child4_pic3 = tk.Label(child4, compound=tk.TOP, image=photo1,text="Dragon: Item #1434" , bg='white')
child4_pic3.place(x=500, y= 60)
##
child4_itemlabel1 = tk.Label(child4,relief='ridge',text="Price :  SG$ 23.00",anchor='w',bg='white')
child4_itemlabel1.place(x=50,y= 260,width=200)
child4_itemlabel2 = tk.Label(child4,relief='ridge',text="Price :  SG$ 23.00",anchor='w',bg='white')
child4_itemlabel2.place(x=280,y= 260,width=200)
child4_itemlabel3 = tk.Label(child4,relief='ridge',text="Price :  SG$ 23.00",anchor='w',bg='white')
child4_itemlabel3.place(x=500,y= 260,width=200)
##
child4_view_item1 = tk.Button(child4, compound=tk.TOP,bg='gray' ,activebackground='yellow', text="View Item #1432", command=child1View1)
child4_view_item1.place(x=90, y= 300)
child4_view_item1 = tk.Button(child4, compound=tk.TOP,bg='gray' , activebackground='yellow', text="View Item #1433", command=child1View2)
child4_view_item1.place(x=320, y= 300)
child4_view_item1 = tk.Button(child4, compound=tk.TOP,bg='gray' ,activebackground='yellow', text="View Item #1434", command=child1View3)
child4_view_item1.place(x=540, y= 300)
##
dragonbuy1 = tk.StringVar()
dragonbuy2 = tk.StringVar()
dragonbuy3 = tk.StringVar()
dragonbuy1.set('Buy')
dragonbuy2.set('Buy')
dragonbuy3.set('Buy')
##
child4_buy1 = tk.Button(child4, compound=tk.TOP,bg='gray' ,activebackground='yellow', textvariable=dragonbuy1, command=buyDragon1)
child4_buy1.place(x=130, y= 350)
child4_buy2 = tk.Button(child4, compound=tk.TOP,bg='gray' , activebackground='yellow', textvariable=dragonbuy2, command=buyDragon2)
child4_buy2.place(x=360, y= 350)
child4_buy3 = tk.Button(child4, compound=tk.TOP,bg='gray' ,activebackground='yellow', textvariable=dragonbuy3, command=buyDragon3)
child4_buy3.place(x=580, y= 350)


#child5
child5 = tk.Toplevel()
child5.geometry('800x600+340+240')
child5.title("The Premier Online Antiques Shop 5")
child5_backbut1 = tk.Button(child5, text="Confirm Purchase -->",command=childToMainWindow,bg='gray',activebackground='yellow')
child5_backbut1.place(x=580, y= 540)
child5.protocol("WM_DELETE_WINDOW", doNothing)
child5.resizable(0,0)
child5.configure(bg="#CCCC99")
child5.withdraw()
##
child5_pic1 = tk.Label(child5, compound=tk.TOP, image=photo2,text="Buddha: Item #1532" , bg='white')
child5_pic1.place(x=90, y= 80)
child5_pic2 = tk.Label(child5, compound=tk.TOP, image=photo2,text="Buddha: Item #1533" , bg='white')
child5_pic2.place(x=320, y= 80)
child5_pic3 = tk.Label(child5, compound=tk.TOP, image=photo2,text="Buddha: Item #1534" , bg='white')
child5_pic3.place(x=540, y= 80)
##
child5_itemlabel1 = tk.Label(child5,relief='ridge',text="Price :  SG$ 1230.00",anchor='w',bg='white')
child5_itemlabel1.place(x=50,y= 350,width=200)
child5_itemlabel2 = tk.Label(child5,relief='ridge',text="Price :  SG$ 1230.00",anchor='w',bg='white')
child5_itemlabel2.place(x=280,y= 350,width=200)
child5_itemlabel3 = tk.Label(child5,relief='ridge',text="Price :  SG$ 1230.00",anchor='w',bg='white')
child5_itemlabel3.place(x=500,y= 350,width=200)
##
child5_view_item1 = tk.Button(child5, compound=tk.TOP,bg='gray' ,activebackground='yellow', text="View Item #1532", command=child2View1)
child5_view_item1.place(x=90, y= 390)
child5_view_item1 = tk.Button(child5, compound=tk.TOP,bg='gray' , activebackground='yellow', text="View Item #1533", command=child2View2)
child5_view_item1.place(x=320, y= 390)
child5_view_item1 = tk.Button(child5, compound=tk.TOP,bg='gray' ,activebackground='yellow', text="View Item #1534", command=child2View3)
child5_view_item1.place(x=540, y= 390)
##
happybuy1 = tk.StringVar()
happybuy2 = tk.StringVar()
happybuy3 = tk.StringVar()
happybuy1.set('Buy')
happybuy2.set('Buy')
happybuy3.set('Buy')
##
child5_buy1 = tk.Button(child5, compound=tk.TOP,bg='gray' ,activebackground='yellow', textvariable=happybuy1, command=buyHappy1)
child5_buy1.place(x=130, y= 440)
child5_buy2 = tk.Button(child5, compound=tk.TOP,bg='gray' , activebackground='yellow', textvariable=happybuy2, command=buyHappy2)
child5_buy2.place(x=360, y= 440)
child5_buy3 = tk.Button(child5, compound=tk.TOP,bg='gray' ,activebackground='yellow', textvariable=happybuy3, command=buyHappy3)
child5_buy3.place(x=580, y= 440)


#child6
child6 = tk.Toplevel()
child6.geometry('800x600+340+240')
child6.title("The Premier Online Antiques Shop 6")
child6_backbut1 = tk.Button(child6, text="Confirm Purchase -->",command=childToMainWindow,bg='gray',activebackground='yellow')
child6_backbut1.place(x=580, y= 540)
child6.protocol("WM_DELETE_WINDOW", doNothing)
child6.resizable(0,0)
child6.configure(bg="#CCCC99")
child6.withdraw()
##
child6_pic1 = tk.Label(child6, compound=tk.TOP, image=photo3,text="Thai Statue: Item #1632" , bg='white')
child6_pic1.place(x=70, y= 60)
child6_pic2 = tk.Label(child6, compound=tk.TOP, image=photo3,text="Thai Statue: Item #1633" , bg='white')
child6_pic2.place(x=300, y= 60)
child6_pic3 = tk.Label(child6, compound=tk.TOP, image=photo3,text="Thai Statue: Item #1634" , bg='white')
child6_pic3.place(x=520, y= 60)
##
child6_itemlabel1 = tk.Label(child6,relief='ridge',text="Price :  SG$ 20,000.00",anchor='w',bg='white')
child6_itemlabel1.place(x=50,y= 400,width=200)
child6_itemlabel2 = tk.Label(child6,relief='ridge',text="Price :  SG$ 20,000.00",anchor='w',bg='white')
child6_itemlabel2.place(x=280,y= 400,width=200)
child6_itemlabel3 = tk.Label(child6,relief='ridge',text="Price :  SG$ 20,000.00",anchor='w',bg='white')
child6_itemlabel3.place(x=500,y= 400,width=200)
##
child6_view_item1 = tk.Button(child6, compound=tk.TOP,bg='gray' ,activebackground='yellow', text="View Item #1632", command=child3View1)
child6_view_item1.place(x=90, y= 440)
child6_view_item1 = tk.Button(child6, compound=tk.TOP,bg='gray' , activebackground='yellow', text="View Item #1633", command=child3View2)
child6_view_item1.place(x=320, y= 440)
child6_view_item1 = tk.Button(child6, compound=tk.TOP,bg='gray' ,activebackground='yellow', text="View Item #1634", command=child3View3)
child6_view_item1.place(x=540, y= 440)
##
thaibuy1 = tk.StringVar()
thaibuy2 = tk.StringVar()
thaibuy3 = tk.StringVar()
thaibuy1.set('Buy')
thaibuy2.set('Buy')
thaibuy3.set('Buy')
##
child6_buy1 = tk.Button(child6, compound=tk.TOP,bg='gray' ,activebackground='yellow', textvariable=thaibuy1, command=buyThai1)
child6_buy1.place(x=130, y= 480)
child6_buy2 = tk.Button(child6, compound=tk.TOP,bg='gray' , activebackground='yellow', textvariable=thaibuy2, command=buyThai2)
child6_buy2.place(x=360, y= 480)
child6_buy3 = tk.Button(child6, compound=tk.TOP,bg='gray' ,activebackground='yellow', textvariable=thaibuy3, command=buyThai3)
child6_buy3.place(x=580, y= 480)

os.system("./delay.sh 200ms")
delay = "400 "
data_rate= "40000"

#all other stuff
text_file = open("log-"+str(datetime.datetime.now())+".txt", "w")
root.mainloop() 
