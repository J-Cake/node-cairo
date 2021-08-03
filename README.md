# node-cairo
A very simple NodeJS Cairo library. It's designed for UIs rather than advanced drawing, so don't rely on it lol 

The library is designed around making drawing UI widgets easier, so there's a bit of text manipulation, rectangles, and some border-radii, but nothing over the top. There's no way to do borders yet, but that's secondary.

## Getting started
If you want to use this library like its intended, you'll want access to your system's framebuffer (Linux Only). My repo `node-fb` achieves this purpose.

```javascript
const dev = require('./fb').open('/dev/fb0'); // If you don't want to write to the framebuffer, you can replace this with a raw image file.
```

Once you're ready to push things to it, open a drawing instance.

```javascript
const canvas = require('./cairo').create(dev);
```

This exposes a number of simple functions.

* `fill([r: number, g: number, b: number])`
  Sets the rectangle or text fill colour
* `rect(rect: Rect)`
  Draws a rectangle, represented by `Rect`
* `text(str: string, props: TextProps): Text`
  Creates a `Text` object
* `flush()`
  Pipe pixel data to device 
* `destroy()` *Not Implemented*
  Unset / dereference the drawing instance
 
### Additional types
* `Rect`: Represents a rectangle
    - `x: number`: x coordinate of the rectangle
    - `y: number`: y coordinate of the rectangle
    - `width: number`: width of the rectangle
    - `height: number`: height of the rectangle
    
* `Text`: Represents intermediate text-drawing tools
    - `width: number`: width of the text string
    - `height: number`: height of the text string
    - `baseline: number`: The height of the baseline (relative to `y`)
    - `bounds(bounds: Rect)`: recalculates the dimensions of the text string
    - `paint(bounds: Rect)`: paint the glyphs to the surface
