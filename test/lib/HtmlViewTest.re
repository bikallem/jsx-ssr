open TestFramework;
open Jsx_ssr.Html;

describe("Html.View", ({test, _}) => {
  test("Output should not be encoded", ({expect, _}) => {
    expect.string(Util.encodeHtml("hello world!")).toEqual("hello world!")
  });

  test("'<' should be encoded.", ({expect, _}) => {
    expect.string(Util.encodeHtml("hello <span")).toEqual("hello &lt;span")
  });
});
