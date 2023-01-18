export const RENDER_BOX = 0

export const HALIGN_CENTER = 0
export const HALIGN_LEFT = 0
export const HALIGN_RIGHT = 0

export const VALIGN_CENTER = 0
export const VALIGN_TOP = 0
export const VALIGN_BOTTOM = 0

export const FLOW_ROW = 0
export const FLOW_COLUMN = 0
export const FLOW_FLEX_ROW = 0
export const FLOW_FLEX_COLUMN = 0

export const BUTTON_HOVERED = 0
export const BUTTON_CLICKED = 0
export const BEHAVIOR_BUTTON = 0

export const POINT_GENERAL = 0
export const POINT_SCREEN_WIDTH = 0
export const POINT_SCREEN_HEIGHT = 0
export const POINT_PARENT_WIDTH = 0
export const POINT_PARENT_HEIGHT = 0

export function addTimer(name: string, sec:number, cb: Function) {}
export function removeTimer(name: string) {}

export class ReactState {
  value = undefined
  constructor(v:any = undefined) {
    this.value = v
  }

}