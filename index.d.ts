declare module 'cairo' {

    export enum TextAlign {
        Left = 0b001,
        Right = 0b011,
        Centre = 0b100,
    }

    export enum TextStyle {
        Bold = 0b0001,
        Italic = 0b0010,
    }

    export interface TextProps {
        font: string,
        size: number,
        align: TextAlign,
        style: TextStyle
    }

    export interface Text {
        width: number,
        height: number,
        baseline: number,
        bounds(bounds: Rect);
        paint(bounds: Rect);
    }

    export interface Drawing {
        fill(colour: RGB),
        rect(rect: Rect);

        text(str: string, props: TextProps): Text;
        
        destroy();
    }

    export type RGB = [r: number, g: number, b: number];

    export type Rect = {
        x: number,
        y: number,
        width: number,
        height: number,
        radius?: number
    };

    export interface Device {
        readonly width: number,
        readonly height: number,

        readonly device: string,

        buffer: Buffer;

        close();
    }
    
    export type paintfn = (pos?: {x: number, y: number}, clip?: {x: number, y: number, width: number, height: number}) => void;
    export type layerfn = (size?: {width: number, height: number}) => Drawing & { paint: paintfn, layer: layerfn };
    
    export type RootLayer = {
        flush();
        layer: layerfn;
        load(device: Device): Drawing & { paint: paintfn, refresh() };
    }

    export function create(dev: Device): Drawing & RootLayer;
}

export namespace cairo {}
