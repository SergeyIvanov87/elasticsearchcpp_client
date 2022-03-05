#!/usr/bin/python3

import tkinter as tk

#import widgets
from tkinter import ttk

#file open dialog
from tkinter import filedialog as fd
from tkinter.messagebox import showinfo

#https://github.com/TkinterEP/ttkwidgets
#from ttkwidgets import CheckboxTreeview

# OS: file size
import os

#import cppyy

filenames = []

root = tk.Tk()
root.title('ES application client')


window_width = 800
window_height = 600

# get the screen dimension
screen_width = root.winfo_screenwidth()
screen_height = root.winfo_screenheight()

# find the center point
center_x = int(screen_width/2 - window_width / 2)
center_y = int(screen_height/2 - window_height / 2)

# set the position of the window to the center of the screen
#root.geometry(f'{window_width}x{window_height}+{center_x}+{center_y}')



# create a notebook
actions_tab = ttk.Notebook(root)
actions_tab.pack(pady=10, expand=True)

# create frames
frame_insert_data = ttk.Frame(actions_tab, width=400, height=280)
frame_search_data = ttk.Frame(actions_tab, width=400, height=280)

### insert data
insert_data_columns = ('file', 'size', 'details')
put_data_treeview = ttk.Treeview(frame_insert_data, columns = insert_data_columns, show='headings')
put_data_treeview.heading('file', text='File Path')
put_data_treeview.heading('size', text='Size')
put_data_treeview.heading('details', text='Details')

def on_double_click(event):
    item = put_data_treeview.selection()
    for i in item:
        print("you clicked on", put_data_treeview.item(i, "values")[0])


put_data_treeview.bind("<Double-1>", on_double_click)
put_data_treeview.pack(expand=True)

# fill frames: insert
def select_files():
    filetypes = (
        ('text files', '*.txt'),
        ('All files', '*.*')
    )

    filenames = fd.askopenfilenames(
        title='Open files',
        initialdir='~',
        filetypes=filetypes)

    for f in filenames:
        stat = os.stat(f);
        put_data_treeview.insert('', tk.END, values = (f, '{:.2f} MB'.format(stat.st_size / (1024 * 1024))));

choose_data_button = ttk.Button(
    frame_insert_data,
    text='Choose Files',
    command=select_files
)
choose_data_button.pack(expand=True, side='left')

# fill frames: clear all
def clear_files():
    filenames = put_data_treeview.get_children()
    for f in filenames:
        put_data_treeview.delete(f);

clear_data_button = ttk.Button(
    frame_insert_data,
    text='Clear Files',
    command=clear_files
)
clear_data_button.pack(expand=True, side='right')

### search data
search_data_columns = ('score', 'id', 'details')
search_data_treeview = ttk.Treeview(frame_search_data, columns = search_data_columns, show='headings')
search_data_treeview.heading('score', text='Score')
search_data_treeview.heading('id', text='ID')
search_data_treeview.heading('details', text='Details')

def on_double_click(event):
    item = search_data_treeview.selection()
    for i in item:
        print("you clicked on", search_data_treeview.item(i, "values")[0])


search_data_treeview.bind("<Double-1>", on_double_click)
search_data_treeview.pack(expand=True)

###
frame_insert_data.pack(fill='both', expand=True)
frame_search_data.pack(fill='both', expand=True)

# add frames to notebook
actions_tab.add(frame_insert_data, text='Insert')
actions_tab.add(frame_search_data, text='Search')

#tree = CheckboxTreeview(root)
#tree.pack()
#tree.insert("", "end", "1", text="1")
#tree.insert("1", "end", "11", text="11")
#tree.insert("1", "end", "12",  text="12")
#tree.insert("11", "end", "111", text="111")
#tree.insert("", "end", "2", text="2")

root.mainloop()
