import * as ui from "@ui"

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
