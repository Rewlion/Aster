import * as ui from "@ui"
import {logerror} from "@log"
import {showMouseCursor} from "@input"

showMouseCursor(true)

let btnState = new ui.ReactState(0)

globalThis.rootUI = {
  size: [500, 500],
  childs: [
    () => ({
      size: btnState.value & ui.BUTTON_CLICKED ? [130, 70] : [100, 50],
      halign: ui.HALIGN_CENTER,
      valign: ui.VALIGN_CENTER,
      render: ui.RENDER_BOX,
      color: btnState.value & ui.BUTTON_HOVERED ? [255, 255, 255] : [0, 255, 255],
      behaviors: [ui.BEHAVIOR_BUTTON],
      observe: [btnState],
      observeBtnState: btnState
    })
  ]
}
