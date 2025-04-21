# -*- coding: utf-8 -*-

import names


def main():
    startApplication("appGroundControlStation")
    
    #initiate takeoff and wait for it to complete
    mouseClick(waitForObject(names.gCS_buttonRect_Rectangle), 90, 68, Qt.LeftButton)
    mouseDrag(waitForObject(names.gCS_Rectangle), 37, 31, 472, 26, Qt.NoModifier, Qt.LeftButton)
    snooze(8)
    
    #initiate landing and wait for it to complete
    mouseClick(waitForObject(names.gCS_buttonRect_Rectangle_2), 81, 39, Qt.LeftButton)
    mouseDrag(waitForObject(names.gCS_Text), 15, 12, 477, 21, Qt.NoModifier, Qt.LeftButton)
    snooze(8)
    
    #validate that the landing text is shown
    test.compare(str(waitForObjectExists(names.gCS_Landed_Text).text), "Landed")
