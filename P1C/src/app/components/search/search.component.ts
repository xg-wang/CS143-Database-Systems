import { Component, OnInit } from '@angular/core';
import { BruimdbServiceService } from '../../services/bruimdb-service.service';
import { Actor } from '../../models/actor';
import { Movie } from '../../models/movie';

@Component({
  selector: 'app-search',
  templateUrl: './search.component.html',
  styleUrls: ['./search.component.css']
})
export class SearchComponent implements OnInit {
  searchStr: string;
  searchActorOption: boolean = true;
  searchMovieOption: boolean = true;
  actorRes: Actor[];
  movieRes: Movie[];

  constructor(private _bruimdbService: BruimdbServiceService) { }

  search() {
    let searchRes = this._bruimdbService.search(this.searchStr,
                                this.searchActorOption,
                                this.searchMovieOption);
    if (this.searchActorOption) {
      searchRes.actors.subscribe(res => {
        this.actorRes = res;
      });
    }
    if (this.searchMovieOption) {
      searchRes.movies.subscribe(res => {
        this.movieRes = res;
      });
    }
  }
  onActorCheckBoxChange(value) {
    this.searchActorOption = value;
  }
  onMovieCheckBoxChange(value) {
    this.searchMovieOption = value;
  }

  ngOnInit() {
  }
}
