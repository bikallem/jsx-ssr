type htmlNode =
  | Text(string)
  | Void(htmlElement)
  | Node(htmlElement, array(htmlNode))

and htmlElement = {
  tag: string,
  attributes: array(htmlAttribute),
}

and htmlAttribute =
  | KeyValue(string, string)
  | Boolean(string);

let str = txt => Text(txt);

let createElement = (tag, attributes, children) => {
  Node({tag, attributes}, children);
};
