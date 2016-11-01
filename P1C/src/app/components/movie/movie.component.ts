import { Component, OnInit } from '@angular/core';
import { Movie } from '../../models/movie';
import { ActivatedRoute } from '@angular/router';
import { BruimdbServiceService } from '../../services/bruimdb-service.service';
import { ActorMovieRelation } from '../../models/actor-movie-relation';
import { Review } from '../../models/review';

@Component({
  selector: 'app-movie',
  templateUrl: './movie.component.html',
  styleUrls: ['./movie.component.css']
})
export class MovieComponent implements OnInit {
  movieInfo: Movie;
  amRelations: ActorMovieRelation[];
  genres: string[];
  reviews: Review[];
  aveRating: number;

  constructor(private _bruimdbService: BruimdbServiceService,
              private _route: ActivatedRoute) { }

  ngOnInit() {
    this._route.params
      .map(params => params['id'])
      .subscribe(id => {
        this._bruimdbService.searchMovieByName(id)
          .subscribe(res => {
            this.movieInfo = res.movieInfo;
            this.amRelations = res.amRelations;
            this.genres = res.genres;
            this.reviews = res.reviews;
            this.aveRating = res.aveRating;
          });
      });
  }
}
