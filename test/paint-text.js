
const lib = require('../cairo.node');

/// <path reference="./index.d.ts">
const surface = lib.create(1024, 768);
const paint = surface.text('hello', {
    size: 32,
    font: 'Raleway'
});
