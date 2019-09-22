open Revery;
open Revery.UI;
open Revery.UI.Components;

let component = React.component("SimpleButton");

let createElement =
    (
      ~onClick,
      ~active: bool=false,
      ~text,
      ~children as _: list(React.syntheticElement),
      (),
    ) =>
  component(hooks => {
    let bgColor =
      active ? Color.rgba(1., 1., 1., 0.75) : Color.rgba(0.5, 0.5, 0.5, 0.5);
    let wrapperStyle =
      Style.[
        backgroundColor(bgColor),
        width(75),
        flexGrow(1),
        alignItems(`Center),
        justifyContent(`Center),
        top(-10),
        marginRight(5),
      ];

    let textHeaderStyle =
      Style.[
        color(Color.hex(active ? "#000000" : "#ffffff")),
        fontFamily("Roboto-Regular.ttf"),
        fontSize(14),
        marginTop(2),
      ];

    (
      hooks,
      <Clickable onClick>
        <View style=wrapperStyle> <Text style=textHeaderStyle text /> </View>
      </Clickable>,
    );
  });