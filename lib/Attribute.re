type t =
  | KeyValue({
      key: string,
      value: string,
    })
  | Boolean(string);

let attr = (key, value) =>
  KeyValue({key, value: Encoder.encodeHtml(value)});
let flag = key => Boolean(key);

let pp = fmt =>
  fun
  | KeyValue({key, value}) => Format.fprintf(fmt, "%s=\"%s\"@.", key, value)
  | Boolean(k) => Format.fprintf(fmt, "%s@.", k);
