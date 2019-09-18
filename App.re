open Revery;
open Revery.Math;
open Revery.UI;
open Revery.UI.Components;

let simpleButton = {
  let component = React.component("SimpleButton");

  (~children as _: list(React.syntheticElement), ()) =>
    component(hooks => {
      let onClick = () => {
        Audio.play();
        ();
      };

      let wrapperStyle =
        Style.[
          backgroundColor(Color.rgba(1., 1., 1., 0.1)),
          border(~width=2, ~color=Colors.white),
          margin(16),
        ];

      let textHeaderStyle =
        Style.[
          color(Colors.white),
          fontFamily("Roboto-Regular.ttf"),
          fontSize(20),
        ];

      let textContent = "Play";
      (
        hooks,
        <Clickable onClick>
          <View style=wrapperStyle>
            <Padding padding=4>
              <Text style=textHeaderStyle text=textContent />
            </Padding>
          </View>
        </Clickable>,
      );
    });
};

let init = app => {
  let _ = Revery.Log.listen((_, msg) => print_endline("LOG: " ++ msg));

  let win = App.createWindow(app, "Welcome to Revery!");

  let containerStyle =
    Style.[
      position(`Absolute),
      justifyContent(`Center),
      alignItems(`Center),
      bottom(0),
      top(0),
      left(0),
      right(0),
    ];

  let innerStyle = Style.[flexDirection(`Row), alignItems(`FlexEnd)];
  let textHeaderStyle =
    Style.[
      color(Colors.white),
      fontFamily("Roboto-Regular.ttf"),
      fontSize(24),
    ];
  let element =
    <View style=containerStyle>
      <View style=innerStyle>
        <Text style=textHeaderStyle text="Welcome" />
      </View>
      <simpleButton />
    </View>;

  let _ = UI.start(win, element);
  ();
};

App.start(init);