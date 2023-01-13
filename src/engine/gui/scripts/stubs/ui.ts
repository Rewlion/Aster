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

export function addTimer(name: string, sec:number, cb: Function) {}
export function removeTimer(name: string) {}

export class ReactState {
  value = undefined
  constructor() {
  }

}