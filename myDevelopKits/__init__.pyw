from PyQt5 import  QtWidgets
from PyQt5 import sip

import sys
sys.path.append(".")

from ui.mainwindow import MainWindow

if __name__ == "__main__":
    import sys
    import os
    
    path = os.path.dirname(os.path.realpath(__file__))
    print("path:" + path)
    os.chdir(path)
    app = QtWidgets.QApplication(sys.argv)
    MainWindows=MainWindow()
    MainWindows.show()
    sys.exit(app.exec_())



