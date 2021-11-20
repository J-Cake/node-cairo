declare module 'cairo' {

    export interface Device {
        data: Buffer,
        width: number,
        height: number,
        device?: string,
    }

    export type Rect = {
        x: number,
        y: number,
        width: number,
        height: number,
        radius?: number
    };

    export type Size = {
        width: number,
        height: number
    }

    export type Pos = {
        x: number,
        y: number
    }

    export const enum TextStyle {
        Bold = 0b0001,
        Italic = 0b0010,
        Underlined = 0b0100,
        Strikethrough = 0b1000
    }

    export const enum TextAlign {
        Left = 0b001,
        Top = 0b010,
        Centre = 0b100,
        Right = 0b011,
        Bottom = 0b110,
        Justify = 0b111
    }

    export type TextProperties = {
        font: string,
        size: number,
        align: TextAlign,
        style: TextStyle
    };

    export interface Text {
        width: number,
        height: number,
        baseline: number,

        bounds(rect: Rect),
        paint(rect: Rect)
    }

    export interface Ctx {
        width: number,
        height: number,

        fill(colour: [r: number, g: number, b: number]),

        rect(rect: Rect),
        text(text: string, TextProperties): Text

        layer(size?: Size): Layer,
        load(device: Device): ExternalLayer
    }

    export interface ExternalLayer {
        paint(pos?: Pos, clip?: Rect),
        refresh()
    }

    export interface Layer extends Ctx {
        paint(pos?: Pos, clip?: Rect)
    }

    export interface Root extends Ctx {
        flush();
        destroy();
    }

    export function create(device: Device): Root

}
