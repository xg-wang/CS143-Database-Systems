import { Component, OnInit } from '@angular/core';

@Component({
  selector: 'app-search',
  templateUrl: './search.component.html',
  styleUrls: ['./search.component.css']
})
export class SearchComponent implements OnInit {
  searchStr: string;
  searchActor: boolean = true;
  searchMovie: boolean = true;

  constructor() { }

  ngOnInit() {
  }

  search() {
    console.log('searching ' + this.searchStr + '!');
  }

}
