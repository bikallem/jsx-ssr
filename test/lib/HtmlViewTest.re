open TestFramework;
open Jsx_ssr;

describe("Html.View", ({test, _}) => {
  test("Output should not be encoded", ({expect, _}) => {
    expect.string(Encoder.encodeHtml("hello world!")).toEqual(
      "hello world!",
    )
  });

  test("'<' should be encoded.", ({expect, _}) => {
    expect.string(Encoder.encodeHtml("hello <span")).toEqual(
      "hello &lt;span",
    )
  });

  test("'>' should be encoded.", ({expect, _}) => {
    expect.string(Encoder.encodeHtml("hello span>")).toEqual(
      "hello span&gt;",
    )
  });

  test("'&' should be encoded", ({expect, _}) => {
    expect.string(Encoder.encodeHtml("hello & world")).toEqual(
      "hello &amp; world",
    )
  });

  test("' should be encoded", ({expect, _}) => {
    expect.string(Encoder.encodeHtml("hello 'world")).toEqual(
      "hello &#x27;world",
    )
  });

  test("\" should be encoded", ({expect, _}) => {
    expect.string(Encoder.encodeHtml("hello \"world")).toEqual(
      "hello &quot;world",
    )
  });

  test("'/' should be encoded", ({expect, _}) => {
    expect.string(Encoder.encodeHtml("hello / world")).toEqual(
      "hello &#x2F; world",
    )
  });
});
