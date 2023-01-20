import * as ui from "@ui"
import {logerror} from "@log"
import {showMouseCursor} from "@input"

showMouseCursor(true)

let btnState = new ui.ReactState(0, "btnState")
let i = 0
let btnHoveredColor = new ui.ReactState([255,255,255], "hoveredColor")

globalThis.rootUI = {
  size: [500, 300],
  render: ui.RENDER_BOX,
  color: [255,255,255,50],
  //clipChilds: true,
  childs: [
    {
      size: [ui.pw(50), ui.ph(50)],
      render: ui.RENDER_BOX,
      color: [50,100,0,255],
    },
    () => ({
      size: btnState.value & ui.BUTTON_CLICKED ? [130, 70] : [100, 50],
      halign: ui.HALIGN_CENTER,
      valign: ui.VALIGN_CENTER,
      render: ui.RENDER_BOX,
      color: btnState.value & ui.BUTTON_HOVERED ? btnHoveredColor.value : [0, 255, 255],
      behaviors: [ui.BEHAVIOR_BUTTON],
      observe: [btnState, btnHoveredColor],
      observeBtnState: btnState,
      onClick: () => {
        btnHoveredColor.value = i % 2 ? [255,255,255] : [255,0,255]
        i++
      },
      childs: {
        size: [ui.pw(100), ui.ph(100)],
        render:ui.RENDER_TEXT,
        text: "BUTTON",
        textHalign: ui.HALIGN_CENTER,
        textValign: ui.VALIGN_CENTER,
        fontSize: ui.ph(30),
        color: [255,0,0],
      }
    })
  ]
}
