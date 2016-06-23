/* plasma5-applet-buam -- Books from your university libray
 * Copyright © 2016 Michał Goliński
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>. */

import QtQuick 2.0
import QtQuick.Controls 1.0 as QtControls
import QtQuick.Dialogs 1.1 as QtDialogs
import QtQuick.Layouts 1.0 as QtLayouts
import QtQuick.Controls.Styles 1.3 as QtStyles


Item {
    id: credentialsPage

    property alias cfg_cardNumber: cardNumber.text
    property alias cfg_peselNumber: peselNumber.text

    QtLayouts.GridLayout {
        columns: 2

        QtControls.Label {
            text: "Numer karty bibliotecznej:"
        }
        QtControls.TextField {
            id: cardNumber
            text: cfg_cardNumber
        }
        QtControls.Label {
            text: "Numer PESEL:"
        }
        QtControls.TextField {
            id: peselNumber
            text: cfg_peselNumber
        }
    }
}
