# -*- coding: utf-8 -*-

import names


def main():
    startApplication("appGroundControlStation")
    
    #perform a take off
    mouseClick(waitForObject(names.gCS_buttonRect_Rectangle), 146, 60, Qt.LeftButton)
    mouseDrag(waitForObject(names.gCS_Text), 6, 8, 662, 36, Qt.NoModifier, Qt.LeftButton)
    
    snooze(6)
    
    #validate state is flying after takeoff complete
    test.compare(str(waitForObjectExists(names.gCS_Flying_Text).text), "Flying")
