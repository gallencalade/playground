#include <vector>

struct Neuron;

template <typename Self>
struct SomeNeurons {
  template <typename T>
  void connect_to(T& other) {
    for (Neuron& from : *static_cast<Self*>(this)) {
      for (Neuron& to : other) {
        from.out.push_back(&to);
        to.in.push_back(&from);
      }
    }
  }
};

struct Neuron : public SomeNeurons<Neuron> {
  std::vector<Neuron*> in, out;
  unsigned int id;

  Neuron() {
    static int id = 1;
    this->id = id++;
  }

  Neuron* begin() { return this; }

  Neuron* end() { return this + 1; }
};

struct NeuronLayer : public std::vector<Neuron>, SomeNeurons<NeuronLayer> {
  NeuronLayer(int count) {
    while (count-- > 0) {
      vec.emplace_back(Neuron{});
    }
  }

  std::vector<Neuron> vec;
};

int main() {
  Neuron neuron, neuron2;
  NeuronLayer layer(2);
  NeuronLayer layer2(3);

  neuron.connect_to(neuron2);
  neuron.connect_to(layer);
  layer.connect_to(neuron);
  layer.connect_to(layer2);

  return 0;
}
