import * as ui from "@ui"

globalThis.rootUI = {
  size: [500, 500],
  render: ui.RENDER_BOX,
  color: [255, 0, 255, 255],
  flow: ui.FLOW_FLEX_COLUMN,
   childs: [
     {
       render: ui.RENDER_BOX,
       color: [0, 255, 0, 255],
     },
     {
      render: ui.RENDER_BOX,
      color: [255, 0, 0, 255],
    },
     {
       flex:2,
       render: ui.RENDER_BOX,
       color: [0, 0, 255],
       childs: [
        {
          size:[40, 40],
          render: ui.RENDER_BOX,
          color: [255, 80, 0],
          halign: ui.HALIGN_RIGHT,
          valign: ui.VALIGN_BOTTOM
        },
        {
          size:[40, 40],
          render: ui.RENDER_BOX,
          color: [255, 80, 50],
          halign: ui.HALIGN_RIGHT
        },
        {
          size:[40, 40],
          render: ui.RENDER_BOX,
          color: [255, 80, 0],
          valign: ui.VALIGN_TOP
        },
        {
          size:[40, 40],
          render: ui.RENDER_BOX,
          color: [255, 80, 0],
          valign: ui.VALIGN_BOTTOM
        },
        {
          size:[200, 200],
          render: ui.RENDER_BOX,
          color: [255,255,255],
          halign: ui.HALIGN_CENTER,
          valign: ui.VALIGN_CENTER,
          flow: ui.FLOW_FLEX_ROW,
          gap:30,
          childs:[
            {
              render: ui.RENDER_BOX,
              color: [0, 0, 0,255],
            },
            {
              flex:3,
              render: ui.RENDER_BOX,
              color: [0, 0, 0, 255],
            },
            {
              render: ui.RENDER_BOX,
              color: [0, 0, 0, 255],
            }
          ]
        }
       ]
     }
   ]
}
