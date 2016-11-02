import { Component, OnInit } from '@angular/core';
import { BruimdbServiceService } from '../../services/bruimdb-service.service';
import { FormData } from '../../models/form-data';

@Component({
  selector: 'app-add-movie',
  templateUrl: './add-movie.component.html',
  styleUrls: ['./add-movie.component.css']
})
export class AddMovieComponent implements OnInit {
  public genres = [
    { 'name': 'Action', 'selected': false },
    { 'name': 'Adult', 'selected': false },
    { 'name': 'Adventure', 'selected': false },
    { 'name': 'Animation', 'selected': false },
    { 'name': 'Comedy', 'selected': false },
    { 'name': 'Crime', 'selected': false },
    { 'name': 'Documentary', 'selected': false },
    { 'name': 'Drama', 'selected': false },
    { 'name': 'Family', 'selected': false },
    { 'name': 'Fantasy', 'selected': false },
    { 'name': 'Horror', 'selected': false },
    { 'name': 'Musical', 'selected': false },
    { 'name': 'Mystery', 'selected': false },
    { 'name': 'Romance', 'selected': false },
    { 'name': 'Sci-Fi', 'selected': false },
    { 'name': 'Short', 'selected': false },
    { 'name': 'Thriller', 'selected': false },
    { 'name': 'War', 'selected': false },
    { 'name': 'Western', 'selected': false }
  ];
  movieForm = new FormData();

  constructor(private _bruimdbService: BruimdbServiceService) { }

  onSubmit() {
    this._bruimdbService.addEntity({
      'entity': 'movie',
      'data': this.movieForm
    }).subscribe(res => {
        console.log(res);
      });
  }
  onSelectGenre(selectedGenre: string, selected: boolean) {
    if (selected) {
      this.movieForm.genre.push(selectedGenre);
    } else {
      let index = this.movieForm.genre.indexOf(selectedGenre, 0);
      if (index > -1) {
        this.movieForm.genre.splice(index, 1);
      }
    }
  }

  ngOnInit() {
  }

}
