import * as ui from "@ui"
import {logerror} from "@log"

let i = 0
ui.addTimer("test_timer", 0.3, () => {
  logerror(`timer ${i}`)
  if (i == 10)
    ui.removeTimer("test_timer")
  i++
})

globalThis.rootUI = {
  size: [500, 500],
  render: ui.RENDER_BOX,
  color: [255, 0, 255, 255],
  flow: ui.FLOW_FLEX_COLUMN,  
  childs: [
    function() {
      return {
        flex:1,
        render: ui.RENDER_BOX,
        color: [0, 255, 0]
      }
    },
    () => ({
      flex: 2,
      render: ui.RENDER_BOX,
      color: [0, 255, 255]
    })
  ]
}
