# -*- coding: utf-8 -*-

"""
Module implementing DataStatisticWidgetCls.
"""


from PyQt5.QtWidgets import QWidget,  QMessageBox, QApplication, QFileDialog, QSizePolicy, QVBoxLayout, QSizePolicy

from PyQt5.QtCore import pyqtSlot,  QTimer

from .Ui_FilesAuto import Ui_FilesAutoForm

import os
import configparser
from datetime import *

from PyQt5 import QtCore
from enum import Enum,  auto
from prompt_dlg  import Prompt_Exec

_translate = QtCore.QCoreApplication.translate

class FilesAutoWidgetCls(QWidget, Ui_FilesAutoForm):
    """
    Class documentation goes here.
    """

    def __init__(self, parent = None):
        """
        Constructor

        @param parent reference to the parent widget
        @type QWidget
        """
        super(FilesAutoWidgetCls, self).__init__()
        self.top_init()
        self.setupUi(self)
        self.parent = parent
        self.usr_init()
        self.show()



    def usr_init(self):
        if self.temp_c_filename:
            self.cTempPathBrowser.setText(self.temp_c_filename)

        if self.temp_h_filename:
            self.hTempPathBrowser.setText(self.temp_h_filename)

        if self.dest_c_dir:
            self.destCpathBrowser.setText(self.dest_c_dir)

        if self.dest_h_dir:
            self.destHpathBrowser.setText(self.dest_h_dir)

        self.BothRadio.setChecked(True)

    def top_init(self):
        self.temp_c_filename = ""
        self.temp_h_filename = ""
        self.dest_c_dir = ""
        self.dest_h_dir = ""

        self.gen_type = GenFCls.BOTHF

        cf = configparser.ConfigParser()
        self.load_f = "./cfg" + '//' + "load.ini"
        cf.read(self.load_f)
        self.temp_c_filename = cf.get("AUTO_GEN_TAB", 'TEMPLATE_C_FILE')

        self.temp_h_filename = cf.get("AUTO_GEN_TAB", 'TEMPLATE_H_FILE')

        self.dest_c_dir = cf.get("AUTO_GEN_TAB", 'DST_C_PATH')

        self.dest_h_dir = cf.get("AUTO_GEN_TAB", 'DST_H_PATH')


    @pyqtSlot()
    def on_cTempChoseBtn_clicked(self):
        filename = QFileDialog.getOpenFileName(self, 'Open file', './templates', "C files (*.c)")
        if (filename[0]):
            self.temp_c_filename = filename[0]
            self.cTempPathBrowser.setText(filename[0])

    @pyqtSlot()
    def on_hTempChoseBtn_clicked(self):
        filename = QFileDialog.getOpenFileName(self, 'Open file', './templates', "H files (*.h)")
        if (filename[0]):
            self.temp_h_filename = filename[0]
            self.hTempPathBrowser.setText(filename[0])

    @pyqtSlot()
    def on_srcFiledirChoseBtn_clicked(self):
        tmpDir = QFileDialog.getExistingDirectory()
        if len(tmpDir) == 0:
            return -1
        self.dest_c_dir = tmpDir
        self.destCpathBrowser.setText(tmpDir);

    @pyqtSlot()
    def on_HeadFiledirChoseBtn_clicked(self):
        tmpDir = QFileDialog.getExistingDirectory()
        if len(tmpDir) == 0:
            return -1
        self.dest_h_dir = tmpDir
        self.destHpathBrowser.setText(tmpDir);

    @pyqtSlot()
    def on_GenBtn_clicked(self):
        cf = configparser.ConfigParser()
        self.load_f = "./cfg" + '//' + "load.ini"
        cf.read(self.load_f)

        if self.NewFileNameEdit.text() == "":
            QMessageBox.warning(self, "ERR", _translate("MainWindow", "file name is empty！"))
            return

        if self.gen_type == GenFCls.BOTHF:
            isExists = os.path.exists(self.temp_c_filename)
            if not isExists:
                QMessageBox.warning(self, "ERR", _translate("MainWindow", "no temp_c_filename！"))
                return

            isExists = os.path.exists(self.temp_h_filename)
            if not isExists:
                QMessageBox.warning(self, "ERR", _translate("MainWindow", "no temp_h_filename！"))
                return

            isExists = os.path.exists(self.dest_c_dir)
            if not isExists:
                QMessageBox.warning(self, "ERR", _translate("MainWindow", "no dest_c_dir！"))
                return

            isExists = os.path.exists(self.dest_h_dir)
            if not isExists:
                QMessageBox.warning(self, "ERR", _translate("MainWindow", "no dest_h_dir！"))
                return

            dstFnameC =  self.dest_c_dir + "//" +self.NewFileNameEdit.text() + ".c"
            dstFnameH = self.dest_h_dir + "//" + self.NewFileNameEdit.text() + ".h"

            isExists = os.path.exists(dstFnameC)
            if isExists:
                QMessageBox.warning(self, "ERR", _translate("MainWindow",  self.NewFileNameEdit.text() + ".c" + " existed!"))
                return

            isExists = os.path.exists(dstFnameH)
            if isExists:
                QMessageBox.warning(self, "ERR",  _translate("MainWindow", self.NewFileNameEdit.text() + ".h" + " existed!"))
                return

            cfgen = CFilsGenCls()
            cfgen.GenCFiles(self.temp_c_filename, dstFnameC, 1, dstFnameH)
            cfgen.GenHFiles(self.temp_h_filename, dstFnameH)


            cf.set("AUTO_GEN_TAB", 'TEMPLATE_C_FILE', self.temp_c_filename)
            cf.set("AUTO_GEN_TAB", 'TEMPLATE_H_FILE', self.temp_h_filename)
            cf.set("AUTO_GEN_TAB", 'DST_C_PATH', self.dest_c_dir)
            cf.set("AUTO_GEN_TAB", 'DST_H_PATH', self.dest_h_dir)
            cf.write(open(self.load_f, "w"))

            Prompt_Exec("Generate finish", 1000)

        elif self.gen_type == GenFCls.CF:
            isExists = os.path.exists(self.temp_c_filename)
            if not isExists:
                QMessageBox.warning(self, "ERR", _translate("MainWindow", "no temp_c_filename！"))
                return

            isExists = os.path.exists(self.dest_c_dir)
            if not isExists:
                QMessageBox.warning(self, "ERR", _translate("MainWindow", "no dest_c_dir！"))
                return


            dstFnameC =  self.dest_c_dir + "//" +self.NewFileNameEdit.text() + ".c"
            isExists = os.path.exists(dstFnameC)
            if isExists:
                QMessageBox.warning(self, "ERR", _translate("MainWindow",  self.NewFileNameEdit.text() + ".c" + " existed!"))
                return

            cfgen = CFilsGenCls()
            cfgen.GenCFiles(self.temp_c_filename, dstFnameC)


            cf.set("AUTO_GEN_TAB", 'TEMPLATE_C_FILE', self.temp_c_filename)
            cf.set("AUTO_GEN_TAB", 'DST_C_PATH', self.dest_c_dir)
            cf.write(open(self.load_f, "w"))
            Prompt_Exec("Generate finish", 1000)

        elif self.gen_type == GenFCls.HF:
            isExists = os.path.exists(self.temp_h_filename)
            if not isExists:
                QMessageBox.warning(self, "ERR", _translate("MainWindow", "no temp_h_filename！"))
                return

            isExists = os.path.exists(self.dest_h_dir)
            if not isExists:
                QMessageBox.warning(self, "ERR", _translate("MainWindow", "no dest_h_dir！"))
                return

            dstFnameH = self.dest_h_dir + "//" + self.NewFileNameEdit.text() + ".h"
            isExists = os.path.exists(dstFnameH)
            if isExists:
                QMessageBox.warning(self, "ERR",  _translate("MainWindow", self.NewFileNameEdit.text() + ".h" + " existed!"))
                return

            cfgen = CFilsGenCls()
            cfgen.GenHFiles(self.temp_h_filename, dstFnameH)

            cf.set("AUTO_GEN_TAB", 'TEMPLATE_H_FILE', self.temp_h_filename)
            cf.set("AUTO_GEN_TAB", 'DST_H_PATH', self.dest_h_dir)
            cf.write(open(self.load_f, "w"))

            Prompt_Exec("Generate finish", 1000)


    def on_BothRadio_toggled(self, checked):
        if checked:
            self.gen_type = GenFCls.BOTHF
            self.cTempBox.setEnabled(True)
            self.destCDirBox.setEnabled(True)
            self.hTempBox.setEnabled(True)
            self.destHDirBox.setEnabled(True)

    def on_CRadio_toggled(self, checked):
        if checked:
            self.gen_type = GenFCls.CF
            self.cTempBox.setEnabled(True)
            self.destCDirBox.setEnabled(True)
            self.hTempBox.setEnabled(False)
            self.destHDirBox.setEnabled(False)

    def on_HRadio_toggled(self, checked):
        if checked:
            self.gen_type = GenFCls.HF
            self.cTempBox.setEnabled(False)
            self.destCDirBox.setEnabled(False)
            self.hTempBox.setEnabled(True)
            self.destHDirBox.setEnabled(True)

class CFilsGenCls():
    def __init__(self):
        self.lines = ["", ]

    def GenCFiles(self, CTempf, CDestf, both = 0, Hname = ""):
        self.lines.clear()
        with open(CTempf, "r") as f:
            for line in f.readlines():
                self.lines.append(line)
                #line.find(" Date:----.--.--")

            line_idx = 0
            for rLine in self.lines:
                ret = rLine.find(" Date:----.--.--")
                if -1 != ret:
                    nowDate= datetime.now().strftime('%Y.%m.%d')  # 现在
                    self.lines[line_idx] = " Date:"+nowDate + "\n"
                    #self.lines[line_idx] = rLine

                ret = rLine.find("TEMPLATE_FILE")
                if -1 != ret:
                    if both:
                        hndef = Hname.split("/")[-1]
                        self.lines[line_idx] = self.lines[line_idx].replace("TEMPLATE_FILE.h", hndef)
                    else:
                        self.lines[line_idx] = "\n"

                line_idx += 1

            with open(CDestf, "w") as f:
                f.writelines(self.lines)
                f.close()

        print(self.lines)


    def GenHFiles(self, HTempf, HDestf):
        self.lines.clear()
        with open(HTempf, "r") as f:
            for line in f.readlines():
                self.lines.append(line)
                #line.find(" Date:----.--.--")

            pathlist = HDestf.split('/')
            hn = pathlist[-1].upper()[0:-2]

            line_idx = 0
            for rLine in self.lines:
                ret = rLine.find("#ifndef __xxxx_H__")
                if -1 != ret:
                    self.lines[line_idx] = "#ifndef __" + hn + "_H__\n"

                ret = rLine.find("#define __xxxx_H__")
                if -1 != ret:
                    self.lines[line_idx] = "#define __" + hn + "_H__\n"

                line_idx += 1

            with open(HDestf, "w") as f:
                f.writelines(self.lines)
                f.close()

        print(self.lines)


class GenFCls(Enum):
    BOTHF = auto()
    CF = auto()
    HF = auto()