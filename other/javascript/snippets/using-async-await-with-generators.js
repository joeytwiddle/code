const delay = ms => new Promise(r => setTimeout(r, ms));


function enterTable(table, name) {
  
}

(async () => {
  const table = {
    players: new Set(),
    state: { hits: 0 },
    async *[Symbol.asyncIterator]() {
      while (true) {
        for (const p of this.players) {
          await p.play(this.state);
          yield this.state;
        }
      }
    }
  };

  table.players.add({
    name: 'ping',
    async play(state) {
      console.log(this.name, state.hits);
      await delay(100);
    }
  });
  table.players.add({
    name: 'pong',
    async play(state) {
      console.log(this.name, state.hits);
      await delay(100);
    }
  });

  for await (const ball of table) {
    ball.hits += 1;
    if (ball.hits > 10) return;
  }
  console.log('done')
})();
