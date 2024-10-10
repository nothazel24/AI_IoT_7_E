class Model:
    def count_finger(self, frame, tanda):
        import cv2
        from cvzone.HandTrackingModule import HandDetector
        import cvzone

        angka = {
            "1": [1, 1, 0, 0, 0],
            "2": [1, 1, 1, 0, 0],
            "3": [1, 1, 1, 1, 0],
            "4": [1, 1, 1, 1, 1],
            "5": [0, 1, 1, 1, 1],
        }

        operasi = {
            '+': lambda x, y: x + y,
            '-': lambda x, y: x - y,
            'x': lambda x, y: x * y,
            '*': lambda x, y: x * y,
            ':': lambda x, y: f"{(x / y):.2f}",
        }

        # tanda = input("Operasi : ")

        handdetec = HandDetector(maxHands=2)

        # cap = cv2.VideoCapture(0)
        #
        # while True:
        #     ret, frame = cap.read()
        frame = cv2.flip(frame, 1)

        hands, img = handdetec.findHands(frame, flipType=False)

        if hands:
            if len(hands) == 2:
                """bikin kondisi agar bisa bedakan kanan kiri"""
                if hands[0]["type"] != hands[1]["type"]:
                    lmlist1 = hands[0] if hands[0]["type"] == "Left" else hands[1]
                    lmlist2 = hands[0] if hands[0]["type"] == "Right" else hands[1]
                    fingerUp1 = handdetec.fingersUp(lmlist1)
                    fingerUp2 = handdetec.fingersUp(lmlist2)
                    value1, value2 = 0, 0
                    for val, isi in angka.items():
                        if fingerUp1 == isi: value1 = int(val)
                        if fingerUp2 == isi: value2 = int(val)
                    hasil = operasi[tanda](value1, value2)

                    cvzone.putTextRect(frame, f"{value1} {tanda} {value2} = {hasil}", pos=(20, 50), scale=3,
                                       thickness=3, colorR=(255, 0, 255), colorT=(255, 255, 255))
        frame = cv2.resize(frame, (1000, 600))
        return frame
