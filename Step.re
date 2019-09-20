open Revery;
open Revery.UI;
open Revery.UI.Components;

let component = React.component("Step");

type buttonColor =
  | Red
  | Orange
  | Yellow
  | White;

let getButtonColor = color =>
  switch (color) {
  | Red => "#D71814"
  | Orange => "#F16F20"
  | Yellow => "#DFDB34"
  | White => "#EAE7C6"
  };

let createElement =
    (~color: buttonColor, ~active: bool, ~onClick, ~children as _, ()) =>
  component(hooks => {
    let bgColor = getButtonColor(color);
    let wrapperStyle =
      Style.[
        backgroundColor(Color.hex(bgColor)),
        alignSelf(`Stretch),
        alignItems(`Stretch),
        flexGrow(1),
        margin(2),
      ];

    let shadowStyle =
      Style.[
        alignSelf(`Stretch),
        height(5),
        backgroundColor(Color.rgba(0., 0., 0., 0.3)),
      ];

    let secondShadowStyle =
      Style.[
        alignSelf(`Stretch),
        height(20),
        backgroundColor(Color.rgba(0., 0., 0., 0.2)),
        justifyContent(`Center),
      ];

    let led =
      Style.[
        backgroundColor(
          active === true ? Color.hex("#ff4444") : Color.hex("#330000"),
        ),
        border(
          ~width=2,
          ~color=
            active === true ? Color.hex("#ff4444") : Color.hex("#330000"),
        ),
        borderRadius(5.),
        height(10),
        width(10),
        alignSelf(`Center),
      ];
    (
      hooks,
      <Clickable style=wrapperStyle onClick>
        <View style=secondShadowStyle> <View style=led /> </View>
        <View style=shadowStyle />
      </Clickable>,
    );
  });