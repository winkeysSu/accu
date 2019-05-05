from PyQt5.QtCore import  QTimer
from PyQt5.QtWidgets import QMessageBox


class PromptDlgCls:
    def __init__(self,  parent = None):
        self.timer = QTimer() #初始化一个定时器
        self.timer.setSingleShot(True)
        self.timer.timeout.connect(self.timer_operate) #计时结束调用

    def prompt_timing(self,  text,  ms):
        self.protptBox = QMessageBox()
        self.protptBox.setStandardButtons(QMessageBox.NoButton)
        self.protptBox.setWindowTitle("Info")
        
        self.timer.start(ms)
        self.protptBox.setText(text)
        self.protptBox.show()
        self.protptBox.setFixedSize(200,  80)
        

         #设置计时间隔并启动
        print(self.protptBox.size())
 
    def timer_operate(self):
        print("time on")
        self.protptBox.hide()

g_prompt_handle = PromptDlgCls()

def Prompt_Exec(text,  ms):
    g_prompt_handle.prompt_timing(text,  ms)
