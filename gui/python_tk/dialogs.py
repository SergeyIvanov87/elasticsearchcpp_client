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

class PropertyDialog(tk.Toplevel):
    non_nonmodified_properties = ['format', 'orig_path']
    unassigned_property_value = "<UNASSIGNED>"

    def __init__(self, parent, PropertyWidgetClass, param_type_names_dict, param_group_names):
        tk.Toplevel.__init__(self)
        self.named_entries = dict()
        self.widget_textvar = dict()
        self.properties = dict()
        r = 1
        for param_type, params_set in param_type_names_dict.items():
            for param_name in params_set:
                if param_name in self.non_nonmodified_properties:
                    continue

                l = tk.Label(self, text=param_name)
                l.grid(row=r, column=1)
                self.widget_textvar[param_name] = tk.StringVar()
                #self.unassigned_property_value)
                self.named_entries[param_name] = PropertyWidgetClass(self, textvariable = self.widget_textvar[param_name])
                self.named_entries[param_name].grid(row=r, column=2)
                #self.named_entries[param_name].bind("KeyPress", self.on_modify)
                r += 1
        Ok = tk.Button(self, text="Ok", command=self.store_setting)
        Cancel = tk.Button(self, text="Cancel", command=self.cancel_setting)
        Ok.grid(row=r, column=1)
        Cancel.grid(row=r, column=2)

        self.parent = parent
        self.acquire_modality()

    def cancel_setting(self):
        self.properties.clear()
        self.release_modality()

    #def on_modify(event):
        #pprint(vars(event))
        #print(event)

    def __del__(self):
        self.release_modality()

    def acquire_modality(self):
        self.wait_visibility()
        self.grab_set()
        self.transient(self.parent)

    def release_modality(self):
        self.grab_release()
        self.destroy()

class PropertyEditor(PropertyDialog):
    apply_properties = False

    def __init__(self, parent, param_type_names_dict, param_group_names):
        PropertyDialog.__init__(self, parent, ttk.Entry, param_type_names_dict, param_group_names)

    def fill(self, json_param_values):
        for param_name,entry in self.named_entries.items():
            if param_name not in json_param_values:
                json_param_values[param_name] = self.unassigned_property_value
            #pprint(vars(entry))
            #entry.insert("end", json_param_values[param_name])
            self.widget_textvar[param_name].set(json_param_values[param_name])

    def store_setting(self):
        for param_name,entry in self.named_entries.items():
            #if param_name not in self.properties:
            #    self.properties[param_name] = ''
            if entry.get() != self.unassigned_property_value:
                #self.properties[param_name] = entry.get()
                self.properties[param_name] = self.widget_textvar[param_name].get()
        self.apply_properties = True
        PropertyDialog.release_modality(self)

    def cancel_setting(self):
        self.apply_properties = False
        PropertyDialog.cancel_setting(self)

    #def on_modify(event):
        #pprint(vars(event))
        #print(event)

    def __del__(self):
        self.apply_properties = False
        PropertyDialog.__del__(self)



class PropertyPrinter(PropertyDialog):

    def __init__(self, parent, param_type_names_dict, param_group_names):
        PropertyDialog.__init__(self, parent, ttk.Label, param_type_names_dict, param_group_names)

    def fill(self, json_param_values):
        r = 1
        for param_name,entry in self.named_entries.items():
            if param_name not in json_param_values:
                json_param_values[param_name] = self.unassigned_property_value
            #entry[param_name] = ttk.Label(self, text = json_param_values[param_name])
            #entry.text(json_param_values[param_name])
            self.widget_textvar[param_name].set(json_param_values[param_name])
        self.update_idletasks()

    def store_setting(self):
        PropertyDialog.release_modality(self)

    def cancel_setting(self):
        PropertyDialog.cancel_setting(self)

    def __del__(self):
        PropertyDialog.__del__(self)