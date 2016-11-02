import { Component, OnInit } from '@angular/core';
import { FormData } from '../../models/form-data';
import { BruimdbServiceService } from '../../services/bruimdb-service.service';

@Component({
  selector: 'app-add-dm-relation',
  templateUrl: './add-dm-relation.component.html',
  styleUrls: ['./add-dm-relation.component.css']
})
export class AddDmRelationComponent implements OnInit {
  relationForm = new FormData();

  constructor(private _bruimdbService: BruimdbServiceService) { }

  onSubmit() {
    this._bruimdbService.addRelation({
      'entity': 'directorMovie',
      'data': this.relationForm
    }).subscribe(res => {
      console.log(res);
    });
  }

  ngOnInit() {
  }

}
