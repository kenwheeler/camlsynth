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
        marginTop(-6),
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
        top(14),
      ];

    let lcdWrapper =
      Style.[
        backgroundColor(Color.hex("#330000")),
        width(50),
        height(30),
        alignItems(`Center),
      ];

    (
      hooks,
      <View style=wrapperStyle>
        <Clickable onClick={onChange(tempo -. 1.)}>
          <View style=buttonStyle>
            <Text style=textButtonStyle text="-" />
          </View>
        </Clickable>
        <View style=lcdWrapper>
          <Text style=lcdText text={string_of_int(int_of_float(tempo))} />
        </View>
        <Clickable onClick={onChange(tempo +. 1.)}>
          <View style=buttonStyle>
            <Text style=textButtonStyle text="+" />
          </View>
        </Clickable>
      </View>,
    );
  });