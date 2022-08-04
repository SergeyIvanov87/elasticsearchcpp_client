#!/usr/bin/python3
import subprocess
import json

import tkinter as tk

#import widgets
from tkinter import ttk
from tkinter import *
from pprint import pprint

#file open dialog
from tkinter import filedialog as fd
from tkinter.messagebox import showinfo

#https://github.com/TkinterEP/ttkwidgets
#from ttkwidgets import CheckboxTreeview

# OS: file size
import os

def fill_schema_params_list(schema_name, out_schema_params_list):
    info_book_process = subprocess.Popen(['./es_info', schema_name],  stdout=subprocess.PIPE, text=True)
    out,err = info_book_process.communicate()
    l = out.split('\n')
    param_list_name = ""
    CONST_PARAM_LIST_NAME_TOKEN = "params list"
    for row in l:
        if CONST_PARAM_LIST_NAME_TOKEN in row:
            param_list_name = row[row.find(CONST_PARAM_LIST_NAME_TOKEN) + len(CONST_PARAM_LIST_NAME_TOKEN):len(row)].strip("\t\" :")
            continue;
        if len(param_list_name) != 0:
            row = row.strip()

            if schema_name not in out_schema_params_list:
                out_schema_params_list[schema_name] = dict()

            out_schema_params_list[schema_name][param_list_name] = row.split(',')
            param_list_name = ""
    return(out_schema_params_list)

def invoke_insert_data_request(filename, schema, properties_dict, force = False):
    print("force: ", force)
    if not force:
        insert_file_process = subprocess.Popen(['./es_put', filename],  stdout=subprocess.PIPE, text=True)
    else:
        insert_file_process = subprocess.Popen(['./es_put', filename, '--force'],  stdout=subprocess.PIPE, text=True)
    out,err = insert_file_process.communicate()
    print(out)
    print(err)
    print(insert_file_process.returncode)
    if insert_file_process.returncode != 0:
        raise RuntimeError(out.split('\n'))
    return err

def fill_file_info(file_path):
    info_process = subprocess.Popen(['./es_test', file_path, '--no_cluster_verbose'],  stdout=subprocess.PIPE, text=True)
    out,err = info_process.communicate()
    l = out.split('\n')

    out_info = dict();
    schema = ""
    CONST_SCHEMA_NAME = "schema"
    for row in l:
        if len(row) == 0:
            continue;
        if CONST_SCHEMA_NAME in row:
            schema = row[row.find(CONST_SCHEMA_NAME) + len(CONST_SCHEMA_NAME):len(row)].strip("\t\" :")
            continue
        value_pair = row.split(': ')
        out_info[value_pair[0]] = value_pair[1]
    return schema,out_info
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
frame_insert_data = ttk.Frame(actions_tab, width=window_width, height=window_height)
frame_search_data = ttk.Frame(actions_tab, width=window_width, height=window_height)

### insert data in frames header
schema_param_names = dict({'book':dict({"must":list(), "filter":list()})})
schema_param_names = fill_schema_params_list('book', schema_param_names)
schema_param_names = fill_schema_params_list('image', schema_param_names)

insert_data_columns = ('file', 'schema', 'size', 'details', 'status')
put_data_treeview = ttk.Treeview(frame_insert_data, columns = insert_data_columns, show='headings')
put_data_treeview.heading('file', text='File Path')
put_data_treeview.heading('schema', text='Schema')
put_data_treeview.heading('size', text='Size')
put_data_treeview.heading('details', text='Details')
put_data_treeview.heading('status', text='Status')

def get_schema_from_insert_item(insert_treeview_data, index):
    return insert_treeview_data.item(index, "values")[1]
#todo return dictionary!
def get_insert_item_data(insert_treeview_data, index):
    json_data = insert_treeview_data.item(index, "values")[3]
    return json.loads(json_data[len("Click to edit\n"): len(json_data)])

def prepare_insert_item_data(data):
    json_object = json.dumps(data, indent = 4)
    return "Click to edit\n" + json_object

def change_insert_item_data(insert_treeview_data, index, new_data):
    item_values = insert_treeview_data.item(index, 'values')
    old_values_dict = get_insert_item_data(insert_treeview_data, index)
    for param, value in new_data.items():
        old_values_dict[param] = value
    insert_treeview_data.item(index, values=(item_values[0], item_values[1], item_values[2], prepare_insert_item_data(old_values_dict), item_values[4]))

def update_insert_item_status(insert_treeview_data, index, new_status_string):
    item_values = insert_treeview_data.item(index, 'values')
    insert_treeview_data.item(index, values=(item_values[0], item_values[1], item_values[2], item_values[3], new_status_string))

class PropertyEditor(tk.Toplevel):
    non_nonmodified_properties = ['format', 'orig_path']
    unassigned_property_value = "<UNASSIGNED>"
    apply_properties = False

    def __init__(self, parent, param_type_names_dict):
        tk.Toplevel.__init__(self)
        self.named_entries = dict()
        self.properties = dict()
        r = 1
        for param_type, params_set in param_type_names_dict.items():
            for param_name in params_set:
                if param_name in self.non_nonmodified_properties:
                    continue

                l = tk.Label(self, text=param_name)
                l.grid(row=r, column=1)
                self.named_entries[param_name] = ttk.Entry(self)
                self.named_entries[param_name].grid(row=r, column=2)
                #self.named_entries[param_name].bind("KeyPress", self.on_modify)
                r += 1
        Ok = tk.Button(self, text="Ok", command=self.store_setting)
        Cancel = tk.Button(self, text="Cancel", command=self.cancel_setting)
        Ok.grid(row=r, column=1)
        Cancel.grid(row=r, column=2)

        self.parent = parent
        self.__acquire_modality()

    def fill(self, json_param_values):
        for param_name,entry in self.named_entries.items():
            if param_name not in json_param_values:
                json_param_values[param_name] = self.unassigned_property_value
            #pprint(vars(entry))
            entry.insert("end", json_param_values[param_name])

    def store_setting(self):
        for param_name,entry in self.named_entries.items():
            #if param_name not in self.properties:
            #    self.properties[param_name] = ''
            if entry.get() != self.unassigned_property_value:
                self.properties[param_name] = entry.get()
        self.apply_properties = True
        self.__release_modality()

    def cancel_setting(self):
        self.apply_properties = False
        self.properties.clear()
        self.__release_modality()

    #def on_modify(event):
        #pprint(vars(event))
        #print(event)

    def __del__(self):
        self.apply_properties = False
        self.__release_modality()

    def __acquire_modality(self):
        self.wait_visibility()
        self.grab_set()
        self.transient(self.parent)

    def __release_modality(self):
        self.grab_release()
        self.destroy()

def on_double_click(event):
    item = put_data_treeview.selection()
    selected_count = len(item)
    new_item_properties = dict()
    if selected_count > 1:
        # collect schema names
        schemas = set()
        for i in item:
            schemas.add(get_schema_from_insert_item(put_data_treeview, i))

        # build common params as all params in schema intersection
        common_param_names = dict()
        for schema in schemas:
            for param_type, param_list in schema_param_names[schema].items():
                if param_type not in common_param_names:
                    common_param_names[param_type] = set(param_list)
                    continue
                common_param_names[param_type] = common_param_names[param_type].intersection(param_list)

        prop_dialog = PropertyEditor(put_data_treeview, common_param_names)
        prop_dialog.wm_title("Group item properties editor")
        put_data_treeview.wait_window(prop_dialog)
        new_item_properties = prop_dialog.properties if prop_dialog.apply_properties else dict()
        del prop_dialog
    elif selected_count == 1:
        specific_item = item[0]
        # if group editor should be selected we find schemas param name intersection mask
        # and apply allowable-for-edit mask
        # and show up final params for editing
        #
        # if single item param selected then we apply allowable-for-edit mask only
        # and show up final params list for editing
        schema = get_schema_from_insert_item(put_data_treeview, specific_item)
        prop_dialog = PropertyEditor(put_data_treeview, schema_param_names[schema])
        prop_dialog.wm_title("Edit '%s' Properties" % get_schema_from_insert_item(put_data_treeview, specific_item))

        prop_dialog.fill(get_insert_item_data(put_data_treeview, specific_item))
        put_data_treeview.wait_window(prop_dialog)
        new_item_properties = prop_dialog.properties if prop_dialog.apply_properties else dict()
        del prop_dialog

    # assign properties for items
    if new_item_properties:
        for i in item:
            change_insert_item_data(put_data_treeview, i, new_item_properties)

put_data_treeview.bind("<Double-1>", on_double_click)
put_data_treeview.pack(expand=True)

# fill frames: insert
def select_files():
    filetypes = (
        ('Supported files', '.JPG .jpg .fb2 .FB2 .epub .EPUB'),
        ('All files', '*.*')
    )

    filenames = fd.askopenfilenames(
        title='Open files',
        initialdir='./',
        filetypes=filetypes)

    for f in filenames:
        stat = os.stat(f);
        schema,info = fill_file_info(f)
        #json_object = json.dumps(info, indent = 4)
        item_values = (f, schema if len(schema) != 0 else "UNSUPPORTED",
                       '{:.2f} MB'.format(stat.st_size / (1024 * 1024)),
                       prepare_insert_item_data(info),
                       "Unknown")
        put_data_treeview.insert('', tk.END, values = item_values);

choose_data_button = ttk.Button(
    frame_insert_data,
    text='Choose Files',
    command=select_files
)
choose_data_button.pack(expand=True, side='left')

def insert_selected_files():
    filenames = put_data_treeview.selection()
    for f in filenames:
        try:
            item_values = put_data_treeview.item(f, 'values')
            invoke_insert_data_request(item_values[0], item_values[1], get_insert_item_data(put_data_treeview, f))
            update_insert_item_status(put_data_treeview, f, "Done")
        except RuntimeError as err:
            print("Bad thing happens during insert files:\n", err)
            if 'Duplicated' in err.args[0][0]:
                update_insert_item_status(put_data_treeview, f, "Failed: Already exist. use 'Force'")
            else:
                update_insert_item_status(put_data_treeview, f, "Failed")


insert_data_button = ttk.Button(
    frame_insert_data,
    text='Insert selected',
    command=insert_selected_files
)
insert_data_button.pack(expand=True, side='left')


def insert_selected_files_force():
    filenames = put_data_treeview.selection()
    for f in filenames:
        try:
            item_values = put_data_treeview.item(f, 'values')
            invoke_insert_data_request(item_values[0], item_values[1], get_insert_item_data(put_data_treeview, f), True)
            update_insert_item_status(put_data_treeview, f, "Done (forced)")
        except RuntimeError as err:
            print("Bad thing happens:\n", err)
            update_insert_item_status(put_data_treeview, f, "Failed")

insert_data_force_button = ttk.Button(
    frame_insert_data,
    text='Force Insert selected',
    command=insert_selected_files_force
)
insert_data_force_button.pack(expand=True, side='left')

# fill frames: clear all
def clear_files():
    filenames = put_data_treeview.selection()
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
