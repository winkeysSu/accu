# -*- coding: utf-8 -*-

"""
Module implementing MainWindow.
"""

from PyQt5.QtCore import pyqtSlot,  QTimer
from PyQt5.QtWidgets import QMainWindow, QMessageBox, QFileDialog,  QApplication,  QScrollArea, QVBoxLayout
from PyQt5 import QtCore, QtWidgets

#import matplotlib.pyplot as plt
#import pandas as pd
import numpy as np
import sys
import os
from datetime import *
import threading

sys.path.append("..")

import time
import serial
import serial.tools.list_ports
import serial.threaded

from ui.Ui_mainwindow import Ui_MainWindow
from .FillsAutoGernerateWidget  import FilesAutoWidgetCls



import logging
import configparser

_translate = QtCore.QCoreApplication.translate

class MainWindow(QMainWindow, Ui_MainWindow):
    """
    Class documentation goes here.
    """

    def __init__(self, parent=None):
        global g_main_win

        """
        Constructor

        @param parent reference to the parent widget
        @type QWidget
        """
        super(MainWindow, self).__init__(parent)
        self.setupUi(self)
        g_main_win = self
        self.will_close = False
        QtCore.QTimer.singleShot(0, self.afterLayout)

    def __del__(self):
        pass

    def afterLayout(self):
        self.tab_cfg()
        self.tabWidget.setCurrentIndex(0)



    def resizeEvent(self, event):
        try:
            print("self.tabWidget.frameGeometry().size()")
            # print(self.tabWidget.frameGeometry().size())
            # if self.rad_test_cf_param == '1':
            #     self.radTestDlg.resize((self.tabWidget.frameGeometry().size()))
            #
            # if self.analyse_cf_param == '1':
            #     self.analyseDlg.resize((self.tabWidget.frameGeometry().size()))
            #
            # if self.rad_correction_cf_param == '1':
            #     print("rad corr")
            #     self.radCorrectionDlg.resize((self.tabWidget.frameGeometry().size()))

        except AttributeError:
            pass

    def top_init(self):
        cf = configparser.ConfigParser()
        self.load_f = "./cfg" + '//' + "load.ini"
        cf.read(self.load_f)



    def tab_cfg(self):
        #self.fileAutoGenDlg = FilesAutoWidgetCls(self.FilesAutoTab)
        #这是不带滚动条的写法
        vbox = QVBoxLayout()
        self.fileAutoGenDlg = FilesAutoWidgetCls(self)
        vbox.addWidget(self.fileAutoGenDlg)
        self.FilesAutoTab.setLayout(vbox)
