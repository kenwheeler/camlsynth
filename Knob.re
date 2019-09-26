/*
 * Knob.re
 *
 * Simple <Knob /> component, based on the <Slider /> component from Revery / react-native
 * Based on: https://github.com/revery-ui/revery/blob/master/src/UI_Components/Slider.re
 */

open Revery;
open Revery.Math;
open Revery.UI;
open Revery.UI.Components;

type clickFunction = unit => unit;
let noop = () => ();

type valueChangedFunction = float => unit;
let noopValueChanged = _f => ();

let component = React.component("Knob");

let createElement =
    (
      ~children as _,
      ~onValueChanged=noopValueChanged,
      ~minimumValue=0.,
      ~maximumValue=1.,
      ~value=0.,
      ~size=64,
      (),
    ) =>
  component(hooks => {
    // We keep a reference to the 'slider' bounding box for calculations with the mouse position
    // This is kind of a carry-over from Revery's slider that I ported over - it's important
    // to have the bounds for a slider, because you want to match up the mouse position with
    // the boundaries. We could get away with less here - just knowing the start position
    // where the 'mousedown' happened, and calculated based on the latest mouse position would
    // be sufficient.
    let (slideRef, setSlideRefOption, hooks) = Hooks.state(None, hooks);

    // Active is used so we can show some custom UI while the
    let (isActive, setActive, hooks) = Hooks.state(false, hooks);

    // Initial value is used to detect if the 'value' parameter ever changes
    let (initialValue, setInitialValue, hooks) = Hooks.state(value, hooks);

    // [v] is the actual value of the slider
    let (v, setV, hooks) = Hooks.state(value, hooks);

    /*
     * If the slider value is updated (controlled),
     * it should override whatever previous value was set
     */
    let v =
      if (value != initialValue) {
        setInitialValue(value);
        setV(value);
        value;
      } else {
        v;
      };

    let setSlideRef = r => setSlideRefOption(Some(r));

    // When the mouse moves, let's figure out the new slider value
    let sliderUpdate = (w, startPosition, endPosition, _mouseX, mouseY) => {
      let mousePosition = mouseY;
      let thumbPosition =
        clamp(mousePosition, startPosition, endPosition) -. startPosition;

      let normalizedValue =
        thumbPosition /. w *. (maximumValue -. minimumValue) +. minimumValue;
      setV(normalizedValue);
      onValueChanged(normalizedValue);
    };

    let sliderComplete = () => {
      setActive(false);
    };

    // We had an 'Always' hook that sets mouse capture - meaning we takeover
    // handling the mouse movement / mouseup events while the gesture is in progress
    // (until 'sliderComplete' is called, or the component unmounts).
    let hooks =
      Hooks.effect(
        Always,
        () => {
          let isCaptured = isActive;

          if (isCaptured) {
            switch (slideRef) {
            | Some(slider) =>
              let w = float_of_int(size);
              let sliderDimensions: BoundingBox2d.t = slider#getBoundingBox();

              let startPosition = Vec2.get_y(sliderDimensions.min);
              let endPosition = startPosition +. w;

              Mouse.setCapture(
                ~onMouseMove=
                  evt =>
                    sliderUpdate(
                      w,
                      startPosition,
                      endPosition,
                      evt.mouseX,
                      evt.mouseY,
                    ),
                ~onMouseUp=_evt => sliderComplete(),
                (),
              );
            | _ => ()
            };
          };

          Some(
            () =>
              if (isCaptured) {
                Mouse.releaseCapture();
              },
          );
        },
        hooks,
      );

    let onMouseDown = (evt: NodeEvents.mouseButtonEventParams) =>
      switch (slideRef) {
      | Some(slider) =>
        let w = float_of_int(size);
        let sliderDimensions: BoundingBox2d.t = slider#getBoundingBox();

        let startPosition = Vec2.get_y(sliderDimensions.min);
        let endPosition = startPosition +. w;

        sliderUpdate(w, startPosition, endPosition, evt.mouseX, evt.mouseY);
        setActive(true);
      | _ => ()
      };

    let sliderOpacity = isActive ? 1.0 : 0.8;

    let normalizedValue =
      (v -. minimumValue) /. (maximumValue -. minimumValue);

    let style =
      Style.[width(size), height(size), cursor(MouseCursors.pointer)];

    let width = size;
    let height = size;

    let sideThumbSize = 2;
    let sideThumbPosition =
      int_of_float(float_of_int(size - sideThumbSize) *. normalizedValue);

    // We'll show a helper slider while the slide-gesture is active
    let sideSlider =
      <Positioned top=0 right=(-2) bottom=0>
        <Positioned top=sideThumbPosition right=0>
          <Container width=8 height=sideThumbSize color=Colors.white />
        </Positioned>
        <Container width=2 height=size color=Colors.white />
      </Positioned>;

    let sliderVisualization = isActive ? sideSlider : React.empty;

    (
      hooks,
      <Opacity opacity=sliderOpacity>
        <View onMouseDown style ref={r => setSlideRef(r)}>
          <Stack width height>
            <Positioned top=0 left=0>
              <Image src="knob-outer.png" width height />
            </Positioned>
            <Positioned top=0 left=0>
              <Image
                src="knob-inner.png"
                width=64
                height=64
                style=Style.[
                  transform([
                    Transform.RotateZ(
                      Angle.from_radians(normalizedValue *. 5.2 -. 2.6),
                    ),
                  ]),
                ]
              />
            </Positioned>
            sliderVisualization
          </Stack>
        </View>
      </Opacity>,
    );
  });
