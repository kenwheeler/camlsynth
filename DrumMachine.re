open Revery;
open Revery.UI;
open Revery.UI.Components;
open Audio;
open Thread;
open Portaudio;
open AppState;

type threadArgs = {
  dispatch: action => unit,
  appStateRef: ref(appState),
};

let component = React.component("DrumMachine");

type streamParams =
  option(Portaudio.stream_parameters(float, Bigarray.float32_elt));

let stream:
  Portaudio.stream(streamParams, streamParams, float, Bigarray.float32_elt) =
  getStream();

let appStateRef = ref(initialAppState);

let createElement = (~children as _, ()) =>
  component(hooks => {
    let (appState, dispatch, hooks) =
      Hooks.reducer(~initialState=initialAppState, reducer, hooks);

    let hooks =
      Hooks.effect(
        Always,
        () => {
          appStateRef := appState;
          None;
        },
        hooks,
      );

    let setActiveStep = step => {
      dispatch(SetActiveStep(step - 1));
    };

    let updateStep = (index, ()) => {
      dispatch(UpdateStep(index));
    };

    let updateTrack = (track, ()) => {
      dispatch(SetActiveTrack(track));
    };

    let playToggle = () => {
      if (appState.playing) {
        setActiveStep(0);
        Audio.stop(stream);
        dispatch(SetPlaying(false));
      } else {
        start_stream(stream);
        let _handle =
          Thread.create(
            ({dispatch, appStateRef}) =>
              Audio.play(stream, dispatch, appStateRef),
            {dispatch, appStateRef},
          );
        dispatch(SetPlaying(true));
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

    let trackStyle =
      Style.[
        alignItems(`FlexStart),
        flexDirection(`Row),
        flexGrow(1),
        marginLeft(20),
        justifyContent(`FlexStart),
      ];

    let currentSteps = appState.tracks[appState.activeTrack].steps;

    (
      hooks,
      <View style=containerStyle>
        <View style=headerStyle>
          <View style=trackStyle>
            <TrackButton
              onClick={updateTrack(0)}
              active={appState.activeTrack == 0}
              text="KICK"
            />
            <TrackButton
              onClick={updateTrack(1)}
              active={appState.activeTrack == 1}
              text="SNARE"
            />
            <TrackButton
              onClick={updateTrack(2)}
              active={appState.activeTrack == 2}
              text="HAT"
            />
          </View>
          <Text style=textHeaderStyle text="Reason Composer" />
          <Text style=subTextStyle text="ML-808" />
        </View>
        <View style=innerStyle>
          <PlayButton playing={appState.playing} onClick=playToggle />
          <View style=stepContainer>
            <View style=stepGroup>
              ...{Array.to_list(
                Array.mapi(
                  (i, s) => {
                    let index = 0 + i;
                    <Step
                      color=Red
                      active={
                        s === 1
                          ? appState.activeStep === index ? false : true
                          : appState.activeStep === index ? true : false
                      }
                      onClick={updateStep(index)}
                    />;
                  },
                  Array.sub(currentSteps, 0, 4),
                ),
              )}
            </View>
            <View style=stepGroup>
              ...{Array.to_list(
                Array.mapi(
                  (i, s) => {
                    let index = 4 + i;
                    <Step
                      color=Orange
                      active={
                        s === 1
                          ? appState.activeStep === index ? false : true
                          : appState.activeStep === index ? true : false
                      }
                      onClick={updateStep(index)}
                    />;
                  },
                  Array.sub(currentSteps, 4, 4),
                ),
              )}
            </View>
            <View style=stepGroup>
              ...{Array.to_list(
                Array.mapi(
                  (i, s) => {
                    let index = 8 + i;
                    <Step
                      color=Yellow
                      active={
                        s === 1
                          ? appState.activeStep === index ? false : true
                          : appState.activeStep === index ? true : false
                      }
                      onClick={updateStep(index)}
                    />;
                  },
                  Array.sub(currentSteps, 8, 4),
                ),
              )}
            </View>
            <View style=stepGroup>
              ...{Array.to_list(
                Array.mapi(
                  (i, s) => {
                    let index = 12 + i;
                    <Step
                      color=White
                      active={
                        s === 1
                          ? appState.activeStep === index ? false : true
                          : appState.activeStep === index ? true : false
                      }
                      onClick={updateStep(index)}
                    />;
                  },
                  Array.sub(currentSteps, 12, 4),
                ),
              )}
            </View>
          </View>
        </View>
      </View>,
    );
  });