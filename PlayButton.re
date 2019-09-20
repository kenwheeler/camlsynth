open Revery;
open Revery.UI;
open Revery.UI.Components;

let component = React.component("SimpleButton");

let createElement =
    (
      ~onClick,
      ~playing: bool,
      ~children as _: list(React.syntheticElement),
      (),
    ) =>
  component(hooks => {
    let wrapperStyle =
      Style.[
        backgroundColor(Color.hex("#EAE7C6")),
        margin(16),
        width(100),
        flexGrow(1),
        alignItems(`Center),
        justifyContent(`Center),
        border(~width=3, ~color=Color.hex("#bdbaa0")),
      ];

    let textHeaderStyle =
      Style.[
        color(Color.hex("#25231E")),
        fontFamily("Roboto-Regular.ttf"),
        fontSize(16),
        marginTop(2),
      ];

    let textContent = playing ? "STOP" : "START";
    (
      hooks,
      <Clickable onClick>
        <View style=wrapperStyle>
          <Text style=textHeaderStyle text=textContent />
        </View>
      </Clickable>,
    );
  });