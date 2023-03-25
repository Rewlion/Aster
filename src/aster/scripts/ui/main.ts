import * as ui from "@ui"
import {logerror} from "@log"
import {showMouseCursor} from "@input"

showMouseCursor(true)

let btnState = new ui.ReactState(0, "btnState")
let btnState2 = new ui.ReactState(0, "btnState2")
let i = 0
let btnHoveredColor = new ui.ReactState([255,255,255], "hoveredColor")

globalThis.rootUI = {
  size: [500, 300],
  render: ui.RENDER_BOX,
  color: [255,255,255,50],
  //clipChilds: true,
  childs: [
    () => ({
      size: btnState.value & ui.BUTTON_CLICKED ? [250, 250] : [200, 200],
      halign: ui.HALIGN_CENTER,
      valign: ui.VALIGN_CENTER,
      render: ui.RENDER_BOX,
      observe: [btnState],
      observeBtnState: btnState,
      behaviors: [ui.BEHAVIOR_BUTTON],
      color: btnState.value & ui.BUTTON_HOVERED ? [100,255,255] : [255,255, 0],
      cursor: true,
      childs: [
        () => ({
          size: btnState2.value & ui.BUTTON_CLICKED ? [80, 80] : [50, 50],
          halign: ui.HALIGN_CENTER,
          valign: ui.VALIGN_CENTER,
          render: ui.RENDER_BOX,
          observe: [btnState2],
          observeBtnState: btnState2,
          behaviors: [ui.BEHAVIOR_BUTTON],
          color: btnState2.value & ui.BUTTON_HOVERED ? [255,255,255] : [255,0, 0]
        })
      ]
    })
  ]
}
