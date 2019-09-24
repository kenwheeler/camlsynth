open Revery;
open Revery.UI;
open Revery.UI.Components;

let component = React.component("Tempo");

let createElement =
    (
      ~onChange,
      ~tempo: float,
      ~children as _: list(React.syntheticElement),
      (),
    ) =>
  component(hooks => {
    let wrapperStyle =
      Style.[
        alignItems(`FlexStart),
        flexDirection(`Row),
        flexGrow(1),
        marginLeft(5),
        justifyContent(`FlexStart),
        marginTop(-8),
      ];

    let buttonStyle =
      Style.[
        padding(2),
        backgroundColor(Color.hex("#EAE7C6")),
        width(25),
        marginHorizontal(4),
        alignItems(`Center),
      ];

    let textButtonStyle =
      Style.[
        color(Color.hex("#25231E")),
        fontFamily("Roboto-Regular.ttf"),
        fontSize(16),
      ];

    let lcdText =
      Style.[
        color(Color.hex("#ff2222")),
        fontFamily("digital-7.ttf"),
        fontSize(28),
        top(15),
      ];

    let lcdWrapper =
      Style.[
        backgroundColor(Color.hex("#330000")),
        width(50),
        height(35),
        alignItems(`Center),
        border(~width=2, ~color=Color.rgba(1., 1., 1., 0.25)),
      ];

    let shadow =
      Style.[
        height(5),
        width(25),
        backgroundColor(Color.hex("#9a9880")),
        marginHorizontal(4),
      ];

    (
      hooks,
      <View style=wrapperStyle>
        <Clickable onClick={onChange(tempo -. 1.)}>
          <View style=buttonStyle>
            <Text style=textButtonStyle text="-" />
          </View>
          <View style=shadow />
        </Clickable>
        <View style=lcdWrapper>
          <Text style=lcdText text={string_of_int(int_of_float(tempo))} />
        </View>
        <Clickable onClick={onChange(tempo +. 1.)}>
          <View style=buttonStyle>
            <Text style=textButtonStyle text="+" />
          </View>
          <View style=shadow />
        </Clickable>
      </View>,
    );
  });