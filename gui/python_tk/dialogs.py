#!/usr/bin/python3
"""
Module exports some auxiliary dialog windows which operate by representations and
editing of properties
"""

import tkinter as tk

# import widgets
from tkinter import ttk
from pprint import pprint
from tkinter import filedialog as fd
from tkinter.messagebox import showinfo

class PropertyDialog(tk.Toplevel):
    """
    Base dialog class with a single task:
    create and groups widgets in a table by pairs: a property name & a property value -
    for all available properties in dictionary/list passed as ctor parameters
    """
    immutable_properties = ["format", "orig_path"]
    unassigned_property_value = "<UNASSIGNED>"

    def __init__(
        self, parent, PropertyWidgetClass, param_type_names_dict, param_group_names
    ):
        tk.Toplevel.__init__(self)
        self.property_widgets = {}
        self.property_widget_textvalue = {}
        self.properties = {}
        row_count = 1
        for param_type, params_set in param_type_names_dict.items():
            for param_name in params_set:
                # hide immutable properties
                if param_name in self.immutable_properties:
                    continue

                # create property name
                property_label = tk.Label(self, text=param_name)
                property_label.grid(row=row_count, column=1)

                # get property values tagged as unassigned/untouched by default
                self.property_widget_textvalue[param_name] = tk.StringVar()
                self.property_widget_textvalue[param_name].set(
                    self.unassigned_property_value
                )

                # create property value by type of PropertyWidgetClass
                self.property_widgets[param_name] = PropertyWidgetClass(
                    self, textvariable=self.property_widget_textvalue[param_name]
                )

                self.property_widgets[param_name].grid(row=row_count, column=2)
                row_count += 1
        Ok = tk.Button(self, text="Ok", command=self.store_setting)
        Cancel = tk.Button(self, text="Cancel", command=self.cancel_setting)
        Ok.grid(row=row_count, column=1)
        Cancel.grid(row=row_count, column=2)

        self.parent = parent
        self.acquire_modality()

    def cancel_setting(self):
        """clear entered data before loose focus"""
        self.properties.clear()
        self.release_modality()

    # def on_modify(event):
    # pprint(vars(event))
    # print(event)

    def __del__(self):
        self.release_modality()

    def acquire_modality(self):
        """
        make windows modal and block input for parent windows
        until this child windows interaction finishes
        """
        self.wait_visibility()
        self.grab_set()
        self.transient(self.parent)

    def release_modality(self):
        self.grab_release()
        self.destroy()


class PropertyEditor(PropertyDialog):
    apply_properties = False

    def __init__(self, parent, param_type_names_dict, param_group_names):
        PropertyDialog.__init__(
            self, parent, ttk.Entry, param_type_names_dict, param_group_names
        )

    def fill(self, json_param_values):
        for param_name, entry in self.property_widgets.items():
            if param_name not in json_param_values:
                continue
            self.property_widget_textvalue[param_name].set(
                json_param_values[param_name]
            )

    def store_setting(self):
        for param_name, entry in self.property_widgets.items():
            if entry.get() != self.unassigned_property_value:
                self.properties[param_name] = self.property_widget_textvalue[
                    param_name
                ].get()
        self.apply_properties = True
        PropertyDialog.release_modality(self)

    def cancel_setting(self):
        self.apply_properties = False
        PropertyDialog.cancel_setting(self)

    # def on_modify(event):
    # pprint(vars(event))
    # print(event)

    def __del__(self):
        self.apply_properties = False
        PropertyDialog.__del__(self)


class PropertyPrinter(PropertyDialog):
    def __init__(self, parent, param_type_names_dict, param_group_names):
        PropertyDialog.__init__(
            self, parent, ttk.Label, param_type_names_dict, param_group_names
        )

    def fill(self, json_param_values):
        for param_name, entry in self.property_widgets.items():
            if param_name not in json_param_values:
                continue
            self.property_widget_textvalue[param_name].set(
                json_param_values[param_name]
            )
        self.update_idletasks()

    def store_setting(self):
        PropertyDialog.release_modality(self)

    def cancel_setting(self):
        PropertyDialog.cancel_setting(self)

    def __del__(self):
        PropertyDialog.__del__(self)
