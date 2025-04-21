# -*- coding: utf-8 -*-

import names


def main():
    startApplication("appGroundControlStation")
    
    #take off and wait for complete
    mouseClick(waitForObject(names.gCS_buttonRect_Rectangle), 149, 53, Qt.LeftButton)
    mouseDrag(waitForObject(names.gCS_Rectangle), 40, 30, 506, 20, Qt.NoModifier, Qt.LeftButton)
    snooze(8)
    
    #validate waypoint UI is displayed
    mouseClick(waitForObject(names.gCS_buttonRect_Rectangle_3), 221, 46, Qt.LeftButton)
    test.compare(waitForObjectExists(names.gCS_goToWaypointConfirmation_Rectangle).visible, True)
    
    #Validate that mouse click on map shows the dest icon on the map
    mouseClick(waitForObject(names.gCS_map_GeoMap), 430, 122, Qt.LeftButton)
    test.compare(waitForObjectExists(names.gCS_destIcon_Image).visible, True)
    
    #validate that the route line is shown after confirming the route
    mouseDrag(waitForObject(names.gCS_Text_2), 13, 17, 400, 7, Qt.NoModifier, Qt.LeftButton)
    test.compare(waitForObjectExists(names.gCS_map_GeoMap).visible, True)
    
    #Validate that the state is loitering after the route completes
    snooze(10)
    test.compare(str(waitForObjectExists(names.gCS_Loitering_Text).text), "Loitering")
