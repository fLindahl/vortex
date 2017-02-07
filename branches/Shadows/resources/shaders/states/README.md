# Render States
Used for telling the renderer how to render an object.

## Usage
There are a bunch of different settings for render states.

---
**CullFace**

*Specifies if we should cull any faces of a primitive*

Accepts values `true` and `false`.

---
**FrontFace**

*Specifies which winding order is the front face of a polygon*

Accepts values `cw` for clockwise order and `ccw` counter-clockwise order.

---
**CullMode**

*Specifies which face of a polygon to cull*

Accepts values `back`, `front` and `front_and_back`

---
**Blend**

*Specifies if we should blend colors when writing with transparency*

Accepts values `true` and `false`.

---
**BlendSourceFunc**

*Specifies which function to use when blending source color*

Accepts values `zero`, `one`, `src_color`, `one_minus_src_color`, `dst_color`, `one_minus_dst_color`, `src_alpha`, `one_minus_src_alpha`, `dst_alpha`, `one_minus_dst_alpha`, `constant_color`, `one_minus_constant_color`, `constant_alpha`, `one_minus_constant_alpha` and `src_alpha_saturate`.

See [glBlendFunc](https://www.opengl.org/sdk/docs/man/html/glBlendFunc.xhtml) for more information.

---
**BlendDestinationFunc**

*Specifies which function to use when blending destination color*

Accepts values `zero`, `one`, `src_color`, `one_minus_src_color`, `dst_color`, `one_minus_dst_color`, `src_alpha`, `one_minus_src_alpha`, `dst_alpha`, `one_minus_dst_alpha`, `constant_color`, `one_minus_constant_color`, `constant_alpha` and `one_minus_constant_alpha`.

See [glBlendFunc](https://www.opengl.org/sdk/docs/man/html/glBlendFunc.xhtml) for more information.

---
**AlphaTest**

*Specifies if we should test alpha to discard fragments*

Accepts values `true` and `false`.

---
**AlphaFunc**

*Specifies which function to use when comparing alpha*

Accepts values `never`, `less`, `equal`, `lequal`, `greater`, `notequal`, `gequal` and `always`.

---
**AlphaClamp**

*Specifies what value is used for alpha testing*

Accepts floating point value between `0.0` and `1.0`.

---
**DepthTest**

*Specifies if we should test depth value to discard fragments early*

Accepts values `true` and `false`.

---
**DepthFunc**

*Specifies which function to use when comparing depth*

Accepts values `never`, `less`, `equal`, `lequal`, `greater`, `notequal`, `gequal` and `always`.

**Note:** Because Vortex uses a depth pre-pass, you might need to use `lequal` to make sure that geometry won't always be discarded the second time we draw.

---
**DepthWrite**

*Specifies if we should write depth value to the depth buffer*

Accepts values `true` and `false`.


## Example
    CullFace = true
    FrontFace = ccw
    CullMode = back
    Blend = false
    BlendSourceFunc = one
    BlendDestinationFunc = zero
    AlphaTest = false
    AlphaFunc = always
    AlphaClamp = 0.0
    DepthTest = true
    DepthFunc = lequal
    DepthWrite = true
