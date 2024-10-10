from PyQt5.QtWidgets import QApplication, QWidget
from PyQt5 import QtCore
from view import View
from model import Model

class Controller(QWidget):
    def __init__(self, model, view):
        super().__init__()
        self.model = model
        self.view = view
        self.tanda = '+'
        self.playing = False
        self.setsylesheet()

    def setsylesheet(self):
        self.view.frame.hide()
        self.view.frame_3.setMaximumSize(QtCore.QSize(16777215, 80))


        self.view.pushButton_4.clicked.connect(lambda: self.stop())

        radioButton = [self.view.radioButton, self.view.radioButton_2, self.view.radioButton_3, self.view.radioButton_4]
        for radio in radioButton:
            radio.clicked.connect(self.check_radioButton)

        self.view.pushButton_7.clicked.connect(lambda: self.load())

    def load(self):
        import cv2
        self.cap = cv2.VideoCapture(0)
        fps = int(self.cap.get(cv2.CAP_PROP_FPS))
        self.playing = True

        self.timer = QtCore.QTimer()
        self.timer.timeout.connect(self.update_frame)
        self.timer.start(fps)

    def update_frame(self):
        ret, frame = self.cap.read()
        if ret:
            frame = self.model.count_finger(frame, self.tanda)
            self.view.update_img(frame)

    def stop(self):
        if self.playing:
            self.cap.release()
            self.view.label_3.setText(" ")
            self.playing = False

    def check_radioButton(self):
        radioButton = {
            self.view.radioButton_2: '+',
            self.view.radioButton_3: '-',
            self.view.radioButton_4: 'x',
            self.view.radioButton: ':',
        }

        self.tanda = next((action for radio, action in radioButton.items() if radio.isChecked()), 'tidak ada')

def main():
    import sys
    app = QApplication(sys.argv)
    model = Model()
    view = View()
    Controller(model, view)
    
    view.setWindowTitle('Telkom University | IEEE System Engineering')
    view.resize(800, 720)
    view.show()

    sys.exit(app.exec_())

if __name__ == '__main__':
    main()