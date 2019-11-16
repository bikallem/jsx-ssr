type t =
  | KeyValue({
      key: string,
      value: string,
    })
  | Boolean(string);

let attr = (key, value) =>
  KeyValue({key, value: Encoder.encodeHtml(value)});
let flag = key => Boolean(key);

let toString =
  Printf.(
    fun
    | KeyValue({key, value}) => sprintf("%s=\"%s\"", key, value)
    | Boolean(k) => sprintf("%s", k)
  );
