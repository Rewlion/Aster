import * as ui from "@ui"
import {logerror} from "@log"
import {showMouseCursor} from "@input"

showMouseCursor(true)

let testState = new ui.ReactState(0)
let i = 0
ui.addTimer("test_timer", 1, () => {
  logerror(`testState.value ${testState.value}`)
  if (i == 10)
  {
    //ui.removeTimer("test_timer")
    i = 0
  }
  i++
  testState.value = i
})

globalThis.rootUI = {
  size: [500, 500],
  render: ui.RENDER_BOX,
  color: [255, 0, 255, 255],
  flow: ui.FLOW_FLEX_COLUMN,  
  childs: [
    function() {
      return {
        observe: [testState],
        flex:1,
        render: ui.RENDER_BOX,
        color: testState.value %2 ? [0, 255, 0] : [255, 255, 0],
      }
    },
    () => ({
      flex: 2,
      render: ui.RENDER_BOX,
      color: [0, 255, 255]
    })
  ]
}
