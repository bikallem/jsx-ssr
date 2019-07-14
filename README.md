# What is JSX SSR?

JSX SSR(Server Side Rendering) is a HTML view engine implemented in ReasonML. As it is inspired by ReasonReact you can use ReasonML JSX syntax to define your html views. However, here the views are executed in the server as opposed to in the client browser. All html generation/rendering is done in native ReasonML and as such there is no requirement of javascript or nodejs.

# v1.0.0 Roadmap

- [ ] Publish to opam and npm
- [ ] Add usage documentation
- [x] Add examples
- [x] Html encode text node
- [x] Add raw text
- [x] Add comment element function
- [x] Add empty element function
- [ ] Add str alias for `text`
- [x] Add unit tests using `ppx_expect`
- [x] Implement JSX ppx for @JSX
- [x] Implement HTML rendering engine
