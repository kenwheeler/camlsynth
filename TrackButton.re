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
    let bgColor = active ? Color.hex("#bdbaa0") : Color.hex("#EAE7C6");
    let wrapperStyle =
      Style.[
        backgroundColor(bgColor),
        width(50),
        flexGrow(1),
        marginRight(1),
        alignItems(`Center),
        justifyContent(`Center),
        top(-10),
      ];

    let textHeaderStyle =
      Style.[
        color(Color.hex("#25231E")),
        fontFamily("Roboto-Regular.ttf"),
        fontSize(14),
        marginTop(2),
      ];

    let shadow =
      Style.[
        height(active ? 0 : 5),
        width(50),
        backgroundColor(Color.hex("#9a9880")),
        top(-10),
      ];

    (
      hooks,
      <Clickable onClick>
        <View style=wrapperStyle> <Text style=textHeaderStyle text /> </View>
        <View style=shadow />
      </Clickable>,
    );
  });