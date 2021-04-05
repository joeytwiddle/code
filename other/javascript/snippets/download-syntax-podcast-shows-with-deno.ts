// deno run --allow-net --allow-write download-shows.ts

import { download } from "https://deno.land/x/download/mod.ts";
const showList = 'https://syntax.fm/api/shows';

async function getShowList(): Promise<Show[]> {
  const list = await (await fetch(showList)).json();
  return list;
}

function generateShowName(show: Show) {
  return `Syntax ${show.number} - ${show.title.replace('/', '')} - ${show.displayDate}.mp3`;
}

async function downloadShow(show: Show) {
  const fileObj = await download(show.url, {
    file: generateShowName(show),
    dir: './shows'
  });
}

async function go() {
  const showList = await getShowList();
  for (const show of showList) {
    console.log(`Downloading show ${generateShowName(show)}`);
    await downloadShow(show);

  }
  console.log('Done!');
}

go();

interface Show {
  number: number;
  title: string;
  date: number;
  url: string;
  slug: string;
  html: string;
  notesFile: string;
  displayDate: string;
  displayNumber: string;
}
