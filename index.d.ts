declare module 'cairo' {

    declare enum TextAlign {
        Left = 0b001,
        Right = 0b011,
        Centre = 0b100,
    }

    declare enum TextStyle {
        Bold = 0b0001,
        Italic = 0b0010,
    }

    declare interface TextProps {
        font: string,
        sizee: number,
        align: TextAlign,
        style: TextStyle
    };

    declare interface Text {
        width: number,
        height: number,
        baseline: number,
        bounds(bounds: Rect);
        paint(bounds: Rect);
    }

    declare interface Drawing {
        fill(colour: RGB),
        rect(rect: Rect);

        text(str: string, props: TextProps): Text;

        flush();
        destroy();
    };

    declare interface Device {
        readonly width: number,
        readonly height: number,

        readonly device: string,

        buffer: Buffer;

        close();
    }

    declare function create(dev: Device): Drawing
};
