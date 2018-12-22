# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'FilesAuto.ui'
#
# Created by: PyQt5 UI code generator 5.11.2
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_FilesAutoForm(object):
    def setupUi(self, FilesAutoForm):
        FilesAutoForm.setObjectName("FilesAutoForm")
        FilesAutoForm.resize(710, 420)
        self.templateGenerateGrp = QtWidgets.QGroupBox(FilesAutoForm)
        self.templateGenerateGrp.setGeometry(QtCore.QRect(20, 20, 671, 381))
        self.templateGenerateGrp.setObjectName("templateGenerateGrp")
        self.GenBtn = QtWidgets.QPushButton(self.templateGenerateGrp)
        self.GenBtn.setGeometry(QtCore.QRect(200, 330, 261, 31))
        self.GenBtn.setObjectName("GenBtn")
        self.BothRadio = QtWidgets.QRadioButton(self.templateGenerateGrp)
        self.BothRadio.setGeometry(QtCore.QRect(20, 60, 81, 16))
        self.BothRadio.setObjectName("BothRadio")
        self.CRadio = QtWidgets.QRadioButton(self.templateGenerateGrp)
        self.CRadio.setGeometry(QtCore.QRect(110, 60, 71, 16))
        self.CRadio.setObjectName("CRadio")
        self.HRadio = QtWidgets.QRadioButton(self.templateGenerateGrp)
        self.HRadio.setGeometry(QtCore.QRect(190, 60, 81, 16))
        self.HRadio.setObjectName("HRadio")
        self.NewFileNameEdit = QtWidgets.QLineEdit(self.templateGenerateGrp)
        self.NewFileNameEdit.setGeometry(QtCore.QRect(70, 19, 181, 31))
        self.NewFileNameEdit.setObjectName("NewFileNameEdit")
        self.label = QtWidgets.QLabel(self.templateGenerateGrp)
        self.label.setGeometry(QtCore.QRect(10, 20, 54, 31))
        self.label.setObjectName("label")
        self.destCDirBox = QtWidgets.QGroupBox(self.templateGenerateGrp)
        self.destCDirBox.setGeometry(QtCore.QRect(350, 90, 311, 101))
        self.destCDirBox.setObjectName("destCDirBox")
        self.destCpathBrowser = QtWidgets.QTextBrowser(self.destCDirBox)
        self.destCpathBrowser.setGeometry(QtCore.QRect(10, 60, 291, 31))
        self.destCpathBrowser.setObjectName("destCpathBrowser")
        self.srcFiledirChoseBtn = QtWidgets.QPushButton(self.destCDirBox)
        self.srcFiledirChoseBtn.setGeometry(QtCore.QRect(10, 20, 191, 31))
        self.srcFiledirChoseBtn.setObjectName("srcFiledirChoseBtn")
        self.destHDirBox = QtWidgets.QGroupBox(self.templateGenerateGrp)
        self.destHDirBox.setGeometry(QtCore.QRect(350, 210, 311, 101))
        self.destHDirBox.setObjectName("destHDirBox")
        self.destHpathBrowser = QtWidgets.QTextBrowser(self.destHDirBox)
        self.destHpathBrowser.setGeometry(QtCore.QRect(10, 60, 291, 31))
        self.destHpathBrowser.setObjectName("destHpathBrowser")
        self.HeadFiledirChoseBtn = QtWidgets.QPushButton(self.destHDirBox)
        self.HeadFiledirChoseBtn.setGeometry(QtCore.QRect(10, 20, 191, 31))
        self.HeadFiledirChoseBtn.setObjectName("HeadFiledirChoseBtn")
        self.cTempBox = QtWidgets.QGroupBox(self.templateGenerateGrp)
        self.cTempBox.setGeometry(QtCore.QRect(10, 90, 311, 101))
        self.cTempBox.setObjectName("cTempBox")
        self.cTempPathBrowser = QtWidgets.QTextBrowser(self.cTempBox)
        self.cTempPathBrowser.setGeometry(QtCore.QRect(10, 60, 291, 31))
        self.cTempPathBrowser.setObjectName("cTempPathBrowser")
        self.cTempChoseBtn = QtWidgets.QPushButton(self.cTempBox)
        self.cTempChoseBtn.setGeometry(QtCore.QRect(10, 20, 181, 31))
        self.cTempChoseBtn.setObjectName("cTempChoseBtn")
        self.hTempBox = QtWidgets.QGroupBox(self.templateGenerateGrp)
        self.hTempBox.setGeometry(QtCore.QRect(10, 210, 311, 101))
        self.hTempBox.setObjectName("hTempBox")
        self.hTempPathBrowser = QtWidgets.QTextBrowser(self.hTempBox)
        self.hTempPathBrowser.setGeometry(QtCore.QRect(10, 60, 291, 31))
        self.hTempPathBrowser.setObjectName("hTempPathBrowser")
        self.hTempChoseBtn = QtWidgets.QPushButton(self.hTempBox)
        self.hTempChoseBtn.setGeometry(QtCore.QRect(10, 20, 171, 31))
        self.hTempChoseBtn.setObjectName("hTempChoseBtn")
        self.hTempBox.raise_()
        self.cTempBox.raise_()
        self.destHDirBox.raise_()
        self.destCDirBox.raise_()
        self.GenBtn.raise_()
        self.BothRadio.raise_()
        self.CRadio.raise_()
        self.HRadio.raise_()
        self.NewFileNameEdit.raise_()
        self.label.raise_()

        self.retranslateUi(FilesAutoForm)
        QtCore.QMetaObject.connectSlotsByName(FilesAutoForm)

    def retranslateUi(self, FilesAutoForm):
        _translate = QtCore.QCoreApplication.translate
        FilesAutoForm.setWindowTitle(_translate("FilesAutoForm", "Form"))
        self.templateGenerateGrp.setTitle(_translate("FilesAutoForm", "TemplatesAutoGenerate"))
        self.GenBtn.setText(_translate("FilesAutoForm", "Generate"))
        self.BothRadio.setText(_translate("FilesAutoForm", "BothFiles"))
        self.CRadio.setText(_translate("FilesAutoForm", ".c File"))
        self.HRadio.setText(_translate("FilesAutoForm", ".h File"))
        self.label.setText(_translate("FilesAutoForm", "FileName:"))
        self.destCDirBox.setTitle(_translate("FilesAutoForm", "Dest c Dir"))
        self.destCpathBrowser.setHtml(_translate("FilesAutoForm", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'SimSun\'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>"))
        self.srcFiledirChoseBtn.setText(_translate("FilesAutoForm", "C Dir Chose"))
        self.destHDirBox.setTitle(_translate("FilesAutoForm", "Dest h Dir"))
        self.destHpathBrowser.setHtml(_translate("FilesAutoForm", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'SimSun\'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>"))
        self.HeadFiledirChoseBtn.setText(_translate("FilesAutoForm", "H Dir Chose"))
        self.cTempBox.setTitle(_translate("FilesAutoForm", "c Template"))
        self.cTempPathBrowser.setHtml(_translate("FilesAutoForm", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'SimSun\'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>"))
        self.cTempChoseBtn.setText(_translate("FilesAutoForm", "C Template"))
        self.hTempBox.setTitle(_translate("FilesAutoForm", "h Template"))
        self.hTempPathBrowser.setHtml(_translate("FilesAutoForm", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:\'SimSun\'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>"))
        self.hTempChoseBtn.setText(_translate("FilesAutoForm", "H Template"))

