#!/usr/bin/python3

#TODO opened for comments! :)

import subprocess
import os
import tkinter as tk

UseOriginalPath = None
DownloadPathPrefix = None
UseRecreateFullFilesPath = None
ReplacePathSeparatorToken = None

#import widgets
from tkinter import ttk
from tkinter import *
from pprint import pprint

#file open dialog
from tkinter import filedialog as fd
from tkinter import messagebox

class GetDataDialog(tk.Toplevel):
    filenames_to_download = dict()

    def __init__(self, parent, filenames):
        tk.Toplevel.__init__(self)

        self.location_options_frame = ttk.LabelFrame(self, text = 'Location', relief=RAISED, borderwidth = 2)
        self.location_options_frame.grid(row = 0, column = 0, padx = 5, pady = 5)

        global UseOriginalPath
        if UseOriginalPath is None:
            UseOriginalPath = tk.IntVar()
            UseOriginalPath.set(1)

        self.UseOriginalPathCheckBox = ttk.Checkbutton(self.location_options_frame, text='Use Original Path',variable=UseOriginalPath, onvalue=1, offvalue=0, command=self.on_use_original_path_click)
        self.UseOriginalPathCheckBox.grid(row = 0, column = 0, padx = 5, pady = 5, ipadx = 5, ipady = 5)

        self.DownloadPathPrefixButton = ttk.Button(self.location_options_frame, text = "Select Base Dir", command = self.on_chose_download_prefix)
        self.DownloadPathPrefixButton.grid(row = 1, column = 0, sticky ='w')

        global DownloadPathPrefix
        if DownloadPathPrefix is None:
            DownloadPathPrefix = tk.StringVar()
            DownloadPathPrefix.set(os.path.expanduser('~'))

        self.DownloadPathPrefixLabel = ttk.Label(self.location_options_frame, textvariable = DownloadPathPrefix)
        self.DownloadPathPrefixLabel.grid(row = 1, column = 1, sticky ='w'+'e'+'n'+'s')

        global UseRecreateFullFilesPath
        if UseRecreateFullFilesPath is None:
            UseRecreateFullFilesPath = tk.IntVar()
            UseRecreateFullFilesPath.set(1)

        self.RecreateFullFilesPath = ttk.Checkbutton(self.location_options_frame, text="Re-create full path",variable=UseRecreateFullFilesPath, onvalue=1, offvalue=0, command=self.on_use_recreate_full_path_click)
        self.RecreateFullFilesPath.grid(row = 2, column = 0, padx = 5, pady = 5, ipadx = 5, ipady = 5)

        self.DownloadPathPrefixDescription = ttk.Label(self.location_options_frame, text = "Replace '\\' token: ")
        self.DownloadPathPrefixDescription.grid(row = 3, column = 0, sticky ='w'+'e'+'n'+'s')

        global ReplacePathSeparatorToken
        if ReplacePathSeparatorToken is None:
            ReplacePathSeparatorToken = tk.StringVar()
            ReplacePathSeparatorToken.set('_')

        self.ReplacePathSeparatorTokenLabel = ttk.Entry(self.location_options_frame, text = 'Token', textvariable = ReplacePathSeparatorToken)
        self.ReplacePathSeparatorTokenLabel.grid(row = 3, column = 1, sticky ='w')

        self.progress_frame = ttk.Frame(self)
        self.progress_frame.grid(row = 1, column = 0, padx = 5, pady = 5, sticky ='w'+'e'+'n'+'s')
        self.downloadProgressBar = ttk.Progressbar(self.progress_frame, orient = 'horizontal', mode='determinate', length = 280)
        self.downloadProgressBar.grid(row = 0, column = 0,  sticky ='w'+'e'+'n'+'s')

        self.action_frame = ttk.Frame(self)
        self.action_frame.grid(row = 2, column = 0, padx = 5, pady = 5, sticky ='w'+'e'+'n'+'s')
        Ok = tk.Button(self.action_frame, text="Download", command=self.on_run)
        Cancel = tk.Button(self.action_frame, text="Cancel", command=self.on_cancel)
        Ok.grid(row=0, column=0)
        Cancel.grid(row=0, column=1)

        # remember files to download
        self.filenames_to_download = filenames

        # Show controls
        self.__show_download_path_control__()
        self.__show_recreate_full_path_control__()

        self.parent = parent
        self.acquire_modality()

    def __show_download_path_control__(self):
        global UseOriginalPath
        self.DownloadPathPrefixLabel["state"] = "disabled" if UseOriginalPath.get() else "normal"
        self.DownloadPathPrefixButton["state"] = "disabled" if UseOriginalPath.get() else "normal"

    def __show_recreate_full_path_control__(self):
        global UseOriginalPath
        global UseRecreateFullFilesPath
        self.RecreateFullFilesPath["state"] = "disabled" if UseOriginalPath.get() else "normal"
        if UseOriginalPath.get():
            self.DownloadPathPrefixDescription["state"] = "disabled"
            self.ReplacePathSeparatorTokenLabel["state"] = "disabled"
        else:
            self.on_use_recreate_full_path_click()
            #self.DownloadPathPrefixDescription["state"] = "disabled" if UseRecreateFullFilesPath.get() else "normal"
            #self.ReplacePathSeparatorTokenLabel["state"] = "disabled" if UseRecreateFullFilesPath.get() else "normal"

    def on_use_original_path_click(self):
        self.__show_download_path_control__()
        self.__show_recreate_full_path_control__()

    def on_use_recreate_full_path_click(self):
        global UseRecreateFullFilesPath
        self.DownloadPathPrefixDescription["state"] = "disabled" if UseRecreateFullFilesPath.get() else "normal"
        self.ReplacePathSeparatorTokenLabel["state"] = "disabled" if UseRecreateFullFilesPath.get() else "normal"

    def on_chose_download_prefix(self):
        global DownloadPathPrefix
        DownloadPathPrefix.set(fd.askdirectory(title='Choose Download Dir', initialdir='./'))

    def on_cancel(self):
        self.release_modality()

    def on_run(self):
        total_document_number = 0
        for k in self.filenames_to_download.keys():
            total_document_number = total_document_number + len(self.filenames_to_download[k])

        progress_percent = 100 / total_document_number
        error_messages = list()
        documents_downloaded = list()
        SKIPPED_COUNT_NAME_TOKEN = "have been skipped"
        ERROR_TOKEN = "Error:"

        global DownloadPathPrefix
        global UseRecreateFullFilesPath

        for schema in self.filenames_to_download.keys():
            download_process = subprocess.Popen(['./es_get', schema] + self.filenames_to_download[schema] +
                                                 ["--extract-path=" + str(DownloadPathPrefix.get()),
                                                  "--use-original-path=" + str(UseRecreateFullFilesPath.get()),
                                                  "--no_cluster_verbose"],
                                                 stdout=subprocess.PIPE, text=True,
                                                 stderr=subprocess.STDOUT)
            out,err = download_process.communicate()
            l = out.split('\n')
            index = 0;

            for row in l:
                if not row:
                    continue;

                self.downloadProgressBar['value'] += progress_percent;
                if SKIPPED_COUNT_NAME_TOKEN in row:
                    skipped_count = int(row[row.find(SKIPPED_COUNT_NAME_TOKEN) + len(SKIPPED_COUNT_NAME_TOKEN):len(row)].strip("\t\" :"))
                    continue;
                if ERROR_TOKEN in row:
                    error_messages.append(row + "\n")
                    continue;

                d = row.split(":")
                documents_downloaded.append(d[1])

        if error_messages:
            if not documents_downloaded:
                messagebox.showwarning("Error", "Cannot download anything!\nErrors:\n" + '\n'.join(error_messages))
            else:
                messagebox.showwarning("Warning", "Downloaded files:\n" + '\n'.join(documents_downloaded) + "\n\nOne or more files have not been downloaded.\nErrors:\n" + '\n'.join(error_messages))
        else:
            messagebox.showinfo("Info", "Downloaded files:\n" + '\n'.join(documents_downloaded))

    def __del__(self):
        self.release_modality()

    def acquire_modality(self):
        self.wait_visibility()
        self.grab_set()
        self.transient(self.parent)

    def release_modality(self):
        self.grab_release()
        self.destroy()
