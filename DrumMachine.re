open Revery;
open Revery.UI;
open Revery.UI.Components;
open Audio;
open Thread;
open Portaudio;
open Lwt;

type threadArgs = {
  setActiveStep: int => unit,
  stepsRef: ref(array(int))
};

let component = React.component("DrumMachine");

type streamParams =
  option(Portaudio.stream_parameters(float, Bigarray.float32_elt));

let stream:
  Portaudio.stream(streamParams, streamParams, float, Bigarray.float32_elt) =
  getStream();

let setActiveRef = ref((v: int) => ());
let stepsRef = ref([|0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0|]);

let createElement = (~children as _, ()) =>
  component(hooks => {
    let (steps, setSteps, hooks) =
      React.Hooks.state(
        [|0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0|],
        hooks,
      );
    let (playing, setPlaying, hooks) = React.Hooks.state(false, hooks);
    let (active, setActive, hooks) = React.Hooks.state(-1, hooks);

    let hooks = Hooks.effect(
      Always,
      () => {
        setActiveRef := setActive;
        stepsRef := steps;
        None;
      },
      hooks
    );

    let setActiveStep = step => {
      setActiveRef^(step - 1);
    };

    let updateStep = (index, ()) => {
      let s = Array.copy(steps);
      s[index] = s[index] === 1 ? 0 : 1;
      setSteps(s);
    };

    let playToggle = () => {
      if (playing) {
        setActiveStep(0);
        Audio.stop(stream);
        setPlaying(false);
      } else {
        start_stream(stream);
        let _handle =
          Thread.create(({setActiveStep, stepsRef}) => Audio.play(stream, setActiveStep, stepsRef), {setActiveStep, stepsRef});
        setPlaying(true);
      };

      ();
    };

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

    let innerStyle =
      Style.[
        flexGrow(1),
        alignSelf(`Stretch),
        flexDirection(`Row),
        alignItems(`FlexEnd),
        backgroundColor(Color.hex("#888888")),
        margin(20),
        borderRadius(5.),
        border(~width=1, ~color=Color.hex("#888888")),
      ];
    let headerStyle =
      Style.[
        flexDirection(`Row),
        justifyContent(`FlexEnd),
        alignSelf(`Stretch),
        paddingTop(20),
        paddingRight(20),
        borderBottom(~width=1, ~color=Color.hex("#F16F20")),
      ];
    let textHeaderStyle =
      Style.[
        color(Color.hex("#F16F20")),
        fontFamily("Roboto-Regular.ttf"),
        fontSize(24),
      ];
    let subTextStyle =
      Style.[
        color(Color.hex("#F16F20")),
        fontFamily("Roboto-Regular.ttf"),
        fontSize(18),
        marginLeft(20),
        marginTop(6),
      ];
    let stepContainer =
      Style.[
        padding(5),
        backgroundColor(Color.hex("#25231E")),
        margin(5),
        flexGrow(1),
        alignSelf(`Stretch),
        flexDirection(`Row),
        alignItems(`Center),
      ];

    let stepGroup =
      Style.[
        flexGrow(1),
        margin(4),
        alignSelf(`Stretch),
        flexDirection(`Row),
        justifyContent(`SpaceBetween),
      ];

    (
      hooks,
      <View style=containerStyle>
        <View style=headerStyle>
          <Text style=textHeaderStyle text="Reason Composer" />
          <Text style=subTextStyle text="ML-808" />
        </View>
        <View style=innerStyle>
          <PlayButton playing onClick=playToggle />
          <View style=stepContainer>
            <View style=stepGroup>
              <Step
                color=Red
                active={(steps[0] === 1 ? true : false) || active === 0}
                onClick={updateStep(0)}
              />
              <Step
                color=Red
                active={(steps[1] === 1 ? true : false) || active === 1}
                onClick={updateStep(1)}
              />
              <Step
                color=Red
                active={(steps[2] === 1 ? true : false) || active === 2}
                onClick={updateStep(2)}
              />
              <Step
                color=Red
                active={(steps[3] === 1 ? true : false) || active === 3}
                onClick={updateStep(3)}
              />
            </View>
            <View style=stepGroup>
              <Step
                color=Orange
                active={(steps[4] === 1 ? true : false) || active === 4}
                onClick={updateStep(4)}
              />
              <Step
                color=Orange
                active={(steps[5] === 1 ? true : false) || active === 5}
                onClick={updateStep(5)}
              />
              <Step
                color=Orange
                active={(steps[6] === 1 ? true : false) || active === 6}
                onClick={updateStep(6)}
              />
              <Step
                color=Orange
                active={(steps[7] === 1 ? true : false) || active === 7}
                onClick={updateStep(7)}
              />
            </View>
            <View style=stepGroup>
              <Step
                color=Yellow
                active={(steps[8] === 1 ? true : false) || active === 8}
                onClick={updateStep(8)}
              />
              <Step
                color=Yellow
                active={(steps[9] === 1 ? true : false) || active === 9}
                onClick={updateStep(9)}
              />
              <Step
                color=Yellow
                active={(steps[10] === 1 ? true : false) || active === 10}
                onClick={updateStep(10)}
              />
              <Step
                color=Yellow
                active={(steps[11] === 1 ? true : false) || active === 11}
                onClick={updateStep(11)}
              />
            </View>
            <View style=stepGroup>
              <Step
                color=White
                active={(steps[12] === 1 ? true : false) || active === 12}
                onClick={updateStep(12)}
              />
              <Step
                color=White
                active={(steps[13] === 1 ? true : false) || active === 13}
                onClick={updateStep(13)}
              />
              <Step
                color=White
                active={(steps[14] === 1 ? true : false) || active === 14}
                onClick={updateStep(14)}
              />
              <Step
                color=White
                active={(steps[15] === 1 ? true : false) || active === 15}
                onClick={updateStep(15)}
              />
            </View>
          </View>
        </View>
      </View>,
    );
  });