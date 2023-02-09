#!/usr/bin/python3
import subprocess
import json

import tkinter as tk

#import widgets
from tkinter import ttk
from tkinter import *
from pprint import pprint
from tkinter import messagebox

#file open dialog
from tkinter import filedialog as fd
from tkinter.messagebox import showinfo

import dialogs
import get_data_dialog
#https://github.com/TkinterEP/ttkwidgets
#from ttkwidgets import CheckboxTreeview

# OS: file size
import os

def init_schema_list(out_schema_list):
    info_process = subprocess.Popen(['./es_info'],  stdout=subprocess.PIPE, text=True)
    out,err = info_process.communicate()
    if info_process.returncode != 0:
        raise RuntimeError(out.split('\n'))
    # schema list is in a table, split by rows
    l = out.split('\n')
    out_schema_list = l[1].strip("\t").split(',')
    if len(out_schema_list) == 0:
        raise RuntimeError("there are no registered schemas");
    return(out_schema_list)


def init_schema_params_list(schema_name, out_schema_params_list):
    info_process = subprocess.Popen(['./es_info', schema_name],  stdout=subprocess.PIPE, text=True)
    out,err = info_process.communicate()
    if info_process.returncode != 0:
        raise RuntimeError(out.split('\n'))

    # schema params list is in a table, split by rows
    l = out.split('\n')
    param_group_name = ""

    # each schema params list is beginning with token and defines a group of params
    CONST_PARAM_LIST_NAME_TOKEN = "params list"
    for row in l:
        if CONST_PARAM_LIST_NAME_TOKEN in row:
            param_group_name = row[row.find(CONST_PARAM_LIST_NAME_TOKEN) + len(CONST_PARAM_LIST_NAME_TOKEN):len(row)].strip("\t\" :")
            continue;
        if len(param_group_name) != 0:
            row = row.strip()

            if schema_name not in out_schema_params_list:
                out_schema_params_list[schema_name] = dict()

            # extract subsequent params in group in list
            if param_group_name == "all":
                out_schema_params_list[schema_name][""] = row.split(',')
            else:
                out_schema_params_list[schema_name][param_group_name] = row.split(',')
            param_group_name = ""
    return(out_schema_params_list)

def union_processor(set_1, set_2):
    return set_1.union(set_2)

def intersection_processor(set_1, set_2):
    return set_1.intersection(set_2)

def get_schema_group_parameters(schemas, schemas_param_names_list, processor_lambda = union_processor):
    # build common params as all params in schema intersection
    unified_param_names = dict()
    # build schema groups list as all groups in schema
    unified_group_list = list()
    for schema in schemas:
        for param_type, param_list in schemas_param_names_list[schema].items():
            #skip empty/all group
            if not param_type:
                continue

            # add functional group only
            if param_type not in unified_param_names:
                unified_param_names[param_type] = set(param_list)
                unified_group_list.append(param_type)
                continue
            unified_param_names[param_type] = processor_lambda(unified_param_names[param_type], param_list)
    return(unified_param_names, unified_group_list)

def invoke_insert_data_request(filename, schema, properties_dict, force = False):
    #TODO apply properties_dict
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

sep_param_value = ':'
sep_params= '\n'

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

#ask API about registered schema
schema_list = list()
schema_list = init_schema_list(schema_list)
schema_param_names = dict()
#and about registered schema params
for schema in schema_list:
    schema_param_names = init_schema_params_list(schema, schema_param_names)

### insert data in frames header
insert_data_columns = ('file', 'schema', 'size', 'details', 'status')
insert_data_columns_shown_test = ('File Path', 'Schema', 'Size', 'Details', 'Status')
put_data_treeview = ttk.Treeview(frame_insert_data, columns = insert_data_columns, show='headings')
for i in range (0, len(insert_data_columns)):
    put_data_treeview.heading(insert_data_columns[i], text=insert_data_columns_shown_test[i])

def get_path_from_insert_item(insert_treeview_data, index):
    for i in range(0, 10000):
        ID = insert_treeview_data.column(i, option='id')
        if ID == insert_data_columns[0]:
            break
    return insert_treeview_data.item(index, "values")[i]

def get_schema_from_insert_item(insert_treeview_data, index):
    for i in range(0, 10000):
        ID = insert_treeview_data.column(i, option='id')
        if ID == "schema":
            break
    return insert_treeview_data.item(index, "values")[i]

#todo return dictionary!
def get_insert_item_data(insert_treeview_data, index):
    for i in range(0, 10000):
        ID = insert_treeview_data.column(i, option='id')
        if ID == "details":
            break

    json_data = insert_treeview_data.item(index, "values")[i]
    return json.loads(json_data[len("Click for details\n"): len(json_data)])

def prepare_insert_item_data(data):
    json_object = json.dumps(data, indent = 4)
    return "Click for details\n" + json_object

def change_insert_item_data(insert_treeview_data, index, new_data):
    item_values = insert_treeview_data.item(index, 'values')
    old_values_dict = get_insert_item_data(insert_treeview_data, index)
    for param, value in new_data.items():
        old_values_dict[param] = value
    insert_treeview_data.item(index, values=(item_values[0], item_values[1], item_values[2], prepare_insert_item_data(old_values_dict), item_values[4]))

def update_insert_item_status(insert_treeview_data, index, new_status_string):
    item_values = insert_treeview_data.item(index, 'values')
    insert_treeview_data.item(index, values=(item_values[0], item_values[1], item_values[2], item_values[3], new_status_string))

def get_name_from_searched_item(search_treeview_data, index):
    for i in range(0, 10000):
        ID = search_treeview_data.column(i, option='id')
        if ID == "id":
            break
    return search_treeview_data.item(index, "values")[i]


def on_double_click_inserted_files(event):
    item = put_data_treeview.selection()
    selected_count = len(item)
    new_item_properties = dict()
    if selected_count > 1:
        # collect collective schema names from files are chosen for insertion
        schemas = set()
        for i in item:
            schemas.add(get_schema_from_insert_item(put_data_treeview, i))

        # build unified param names list as all params from schemas param intersection by the rule:
        # we obliged to edit params only in an `intersection` set of selected schemas params.
        # because it is not reasonable to edit non-existed params values in schema S1 for items belong to S2
        # while collective items in S1+S2 are chosen to be inserted
        # This is quite different schema in comparison with a search-items-list schema, which use `union`
        # for mixed schema params
        common_param_names, common_param_group = get_schema_group_parameters(schemas, schema_param_names, intersection_processor)

        # invoke dialog
        prop_dialog = dialogs.PropertyEditor(put_data_treeview, common_param_names, common_param_group)
        prop_dialog.wm_title("Group item properties editor")
        put_data_treeview.wait_window(prop_dialog)
        new_item_properties = prop_dialog.properties if prop_dialog.apply_properties else dict()
        isOkClicked = prop_dialog.apply_properties;
        del prop_dialog
    elif selected_count == 1:
        specific_item = item[0]
        # get schema from selected item and build schema param list for properties editing
        schema = get_schema_from_insert_item(put_data_treeview, specific_item)
        common_param_names, common_param_group = get_schema_group_parameters( {schema}, schema_param_names)
        prop_dialog = dialogs.PropertyEditor(put_data_treeview, common_param_names, common_param_group)
        prop_dialog.wm_title("Edit '%s' Properties" % get_schema_from_insert_item(put_data_treeview, specific_item))

        prop_dialog.fill(get_insert_item_data(put_data_treeview, specific_item))
        put_data_treeview.wait_window(prop_dialog)
        new_item_properties = prop_dialog.properties if prop_dialog.apply_properties else dict()
        isOkClicked = prop_dialog.apply_properties;
        del prop_dialog

    # assign properties for items
    if not isOkClicked:
        return

    if new_item_properties:
        for i in item:
            change_insert_item_data(put_data_treeview, i, new_item_properties)

put_data_treeview.bind("<Double-1>", on_double_click_inserted_files)
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

    # prepare filter on existed items
    existed_items = put_data_treeview.get_children();
    existed_items_path = set()
    for existed in existed_items:
        existed_items_path.add(get_path_from_insert_item(put_data_treeview, existed))

    for f in filenames:
        # do not insert duplicates
        if f in existed_items_path:
            print(f"duplicated item: {f} - won't be inserted")
            continue

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
            print("Bad things happen during a files insertion:\n", err)
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
def drop_selected_inserted_data_button():
    filenames = put_data_treeview.selection()
    for f in filenames:
        put_data_treeview.delete(f);

clear_data_button = ttk.Button(
    frame_insert_data,
    text='Drop Selected',
    command=drop_selected_inserted_data_button
)
clear_data_button.pack(expand=True, side='left')


# fill frames: clear all
def clear_inserted_files():
    filenames = put_data_treeview.get_children()
    for f in filenames:
        put_data_treeview.delete(f);

clear_inserted_data_button = ttk.Button(
    frame_insert_data,
    text='Clear All',
    command=clear_inserted_files
)
clear_inserted_data_button.pack(expand=True, side='right')

### final pack insert view
frame_insert_data.pack(fill='both', expand=True)

### search data
tools_frame = ttk.Frame(frame_search_data)
tools_frame.grid(row = 0, column = 0, padx = 5, pady = 5)

tools_frame_row_1_search_frame = ttk.LabelFrame(tools_frame, text = 'Search Options', relief=RAISED, borderwidth = 2)
tools_frame_row_1_search_frame.grid(row = 0, column = 0)

search_schema_enable_checkbox_value = dict()
def show_checked_schema_files(name):
    print(search_schema_enable_checkbox_value[name].get())
schemas_search_group = ttk.LabelFrame(tools_frame_row_1_search_frame, text ='schemas', width = 100)

schemas_tools_counter  = 0
for schema in schema_list:
    search_schema_enable_checkbox_value[schema] = IntVar()
    ttk.Checkbutton(
                    schemas_search_group,
                    text=schema,
                    variable=search_schema_enable_checkbox_value[schema],
                    command=lambda schema=schema: show_checked_schema_files(schema)
    ).grid(row = schemas_tools_counter, column = 0)
    schemas_tools_counter = schemas_tools_counter + 1
schemas_search_group.grid(row = 0, column = 0, padx = 5, pady = 10)

search_view_frame = ttk.Frame(frame_search_data)
search_data_columns = ('score', 'id', 'schema', 'details')
search_data_columns_shown_test = ('Score', 'ID', 'Schema', 'Details')
search_data_treeview = ttk.Treeview(search_view_frame, columns = search_data_columns, show='headings')
for i in range (0, len(search_data_columns)):
    search_data_treeview.heading(search_data_columns[i], text=search_data_columns_shown_test[i])

def get_unique_searched_item_params(index):
    #TODO make position independent lookup
    #for i in range(0, 10000):
    #    ID = insert_treeview_data.column(i, option='id')
    #    if ID == insert_data_columns[0]:
    #        break

    values = search_data_treeview.item(index, "values")
    return values[1], values[2], values[3]


def on_double_click_searched_files(event):
    item = search_data_treeview.selection()
    for i in item:
        print("you clicked on", search_data_treeview.item(i, "values")[0])

    selected_count = len(item)
    if selected_count > 1:
        # collect schema names
        schemas = set()
        for i in item:
            schemas.add(get_schema_from_insert_item(search_data_treeview, i))

        # build common params as all params in schema intersection
        common_param_names, common_param_group = get_schema_group_parameters(schemas, schema_param_names)

        prop_dialog = dialogs.PropertyPrinter(search_data_treeview, common_param_names, common_param_group)
        prop_dialog.wm_title("Group item properties editor")
        search_data_treeview.wait_window(prop_dialog)
        del prop_dialog
    elif selected_count == 1:
        specific_item = item[0]
        # if group editor should be selected we find schemas param name intersection mask
        # and apply allowable-for-edit mask
        # and show up final params for editing
        #
        # if single item param selected then we apply allowable-for-edit mask only
        # and show up final params list for editing
        schema = get_schema_from_insert_item(search_data_treeview, specific_item)
        common_param_names, common_param_group = get_schema_group_parameters({schema}, schema_param_names)
        prop_dialog = dialogs.PropertyPrinter(search_data_treeview, common_param_names, common_param_group)
        prop_dialog.wm_title("Edit '%s' Properties" % get_schema_from_insert_item(search_data_treeview, specific_item))

        prop_dialog.fill(get_insert_item_data(search_data_treeview, specific_item))
        search_data_treeview.wait_window(prop_dialog)
        del prop_dialog

search_data_treeview.bind("<Double-1>", on_double_click_searched_files)

### search frames: search files button
def search_files():
    # collect schema names
    schemas = set()
    for schema_name,schema_checked in search_schema_enable_checkbox_value.items():
        print(f"schema name {schema_name!r}, value {schema_checked}")
        if schema_checked.get():
            schemas.add(schema_name)

    if not schemas:
        messagebox.showwarning("Warning", "Please choose schemas to search")
        return

    # Show all parameters for all selected schemas as property editor dialog
    united_schema_param_names, param_group = get_schema_group_parameters(schemas, schema_param_names)
    prop_dialog = dialogs.PropertyEditor(search_data_treeview, united_schema_param_names, param_group)
    prop_dialog.wm_title("Search item properties editor")
    put_data_treeview.wait_window(prop_dialog)
    new_item_properties = prop_dialog.properties if prop_dialog.apply_properties else dict()
    isOkClicked = prop_dialog.apply_properties;
    del prop_dialog;

    # check Ok or Cancel
    if not isOkClicked:
        return

    #TODO Implement search request function
    search_param_string = ""
    for param, value in new_item_properties.items():
        if value:
            search_param_string = "\"" + param + sep_param_value + value + "\""

    search_param_string_per_schema = dict()
    for schema in schemas:
        search_param_string_per_schema[schema] = ""
        for param, value in new_item_properties.items():
            if value and param in schema_param_names[schema][""]:
                if len(search_param_string_per_schema[schema]) == 0:
                    search_param_string_per_schema[schema] = "\"" + param + sep_param_value + value + "\""
                else:
                    search_param_string_per_schema[schema] = search_param_string_per_schema[schema] + "\n\"" + param + sep_param_value + value + "\""

    #files_info = dict({schema_name:dict()})
    files_info = dict()
    for schema in schemas:
        print(f"ask {schema} for params {search_param_string_per_schema[schema]}")
        search_process = subprocess.Popen(['./es_search', schema, search_param_string_per_schema[schema]],  stdout=subprocess.PIPE, text=True)

        out,err = search_process.communicate()
        l = out.split('\n')
        record_count = 0
        record_index = 0;

        RECORDS_COUNT_NAME_TOKEN = "records count"
        for row in l:
            if RECORDS_COUNT_NAME_TOKEN in row:
                record_count = int(row[row.find(RECORDS_COUNT_NAME_TOKEN) + len(RECORDS_COUNT_NAME_TOKEN):len(row)].strip("\t\" :"))
                continue;
            if record_count > record_index + 1:
                file_description = row.split('\t')

                if schema not in files_info:
                    files_info[schema] = dict()

                files_info[schema][file_description[0]] = dict()
                file_property_values = file_description[1].split(',')
                for file_property_key in schema_param_names[schema][""]:
                    files_info[schema][file_description[0]][file_property_key] = file_property_values[len(files_info[schema][file_description[0]].keys())]

                record_index = record_index + 1

    # prepare filter on existed items
    existed_items = search_data_treeview.get_children();
    existed_items_path = set()
    for existed in existed_items:
        existed_items_path.add(get_unique_searched_item_params(existed))

    print(existed_items_path)
    # fill in search frame
    for schema,files_dict in files_info.items():
        for file_name, file_prop in files_dict.items():
            item_values = (0.0, file_name, schema,
                            "Click for details\n" + json.dumps(file_prop, indent = 4)
                            )
            if (item_values[1], item_values[2], item_values[3]) in existed_items_path:
                print(f"duplicated item: {file_name}/{schema} - won't be inserted")
                continue
            search_data_treeview.insert('', tk.END, values = item_values);
    return(files_info)

    #######
tools_frame_row_1_search_frame_buttons_frame = ttk.Frame(tools_frame_row_1_search_frame)
tools_frame_row_1_search_frame_buttons_frame.grid(row = 0, column = 1, padx = 5, pady = 5)
search_data_button = ttk.Button(
    tools_frame_row_1_search_frame_buttons_frame,
    text='Run Search',
    width = 12,
    command=search_files
)
search_data_button.grid(row = 0, column = 0, pady = 2)

# search frames: drop selected
def drop_selected_files():
    filenames = search_data_treeview.selection()
    for f in filenames:
        search_data_treeview.delete(f);

drop_selected_search_data_button = ttk.Button(
    tools_frame_row_1_search_frame_buttons_frame,
    width = 12,
    text='Clear Selected',
    command=drop_selected_files
)
drop_selected_search_data_button.grid(row = 1, column = 0, pady = 2)

# search frames: clear all
def clear_searched_files():
    filenames = search_data_treeview.get_children()
    for f in filenames:
        search_data_treeview.delete(f);

clear_search_data_button = ttk.Button(
    tools_frame_row_1_search_frame_buttons_frame,
    width = 12,
    text='Clear All',
    command=clear_searched_files
)
clear_search_data_button.grid(row = 2, column = 0, pady = 2)



# search frames: get searched files button
tools_frame_row_2_download_frame = ttk.LabelFrame(tools_frame, text = 'Copy Data', relief = RAISED, borderwidth = 2)
tools_frame_row_2_download_frame.grid(row = 1, column = 0, sticky ='w'+'n'+'e'+'s')
def get_searched_files():
    filenames = search_data_treeview.selection()

    if not filenames:
        messagebox.showwarning("Warning", "Please choose files to download")
        return
    schema_files = dict();
    for i in filenames:
        schema = get_schema_from_insert_item(search_data_treeview, i)
        name = get_name_from_searched_item(search_data_treeview, i)
        if schema not in schema_files.keys():
            schema_files[schema] = list()
        schema_files[schema].append(name)

    try:
        download_options = get_data_dialog.GetDataDialog(search_data_treeview, schema_files)
        download_options.wm_title("Download options")
        search_data_treeview.wait_window(download_options)

    except RuntimeError as err:
        print("Bad thing happens:\n", err)

get_searched_files_button = ttk.Button(
    tools_frame_row_2_download_frame,
    text='Run Copy',
    command=get_searched_files
)
get_searched_files_button.grid(row = 1, column = 0, padx = 5, pady = 5)


# search frames: delete files button
tools_frame_row_3_delete_frame = ttk.LabelFrame(tools_frame, text = 'Delete Data', relief = RAISED, borderwidth = 2)
tools_frame_row_3_delete_frame.grid(row = 2, column = 0, sticky ='w'+'n'+'e'+'s')
def delete_stored_files(no_confirm):
    filenames = search_data_treeview.selection()

    if not filenames:
        messagebox.showwarning("Warning", "Please choose files to delete")
        return

    schema_files = dict();
    for i in filenames:
        schema = get_schema_from_insert_item(search_data_treeview, i)
        name = get_name_from_searched_item(search_data_treeview, i)
        if schema not in schema_files.keys():
            schema_files[schema] = list()
        schema_files[schema].append(name)

    file_statuses = list()
    for schema in schema_files.keys():
        for doc in schema_files[schema]:
            delete_process = subprocess.Popen(['./es_rm', schema,
                                               doc,
                                               "--no_cluster_verbose"],
                                               stdout=subprocess.PIPE, text=True,
                                               stderr=subprocess.STDOUT)
            out,err = delete_process.communicate()
            l = out.split('\n')
            for el in l:
                # TODO filter out filename and remove it from table view
                if l:
                    file_statuses = file_statuses + l

    messagebox.showinfo("Info", "Delete operation result:\n\n" + "\n".join(file_statuses))

delete_stored_files_force_value = tk.IntVar()
delete_stored_files_button = ttk.Button(
    tools_frame_row_3_delete_frame,
    text='Run Delete',
    command=lambda delete_stored_files_force_value = delete_stored_files_force_value: delete_stored_files(delete_stored_files_force_value)
)
delete_stored_files_button.grid(row = 1, column = 0, padx = 5, pady = 5)
delete_stored_files_force = ttk.Checkbutton(
                    tools_frame_row_3_delete_frame,
                    text='No Confirm',
                    variable=delete_stored_files_force_value,
                    onvalue=1, offvalue=0,
)
delete_stored_files_force.grid(row = 1, column = 2, padx = 5, pady = 5)

### final pack search view
search_data_treeview.grid(row = 0, column = 0, padx = 5, pady = 5)
search_view_frame.grid(row = 0, column = 1, sticky ='w'+'n'+'e'+'s')
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
